#include <uso/sleep.h>
#include <dev/spi.h>

#include <dpy/nokia6100.h>
#include <dpy/font.h>

#define MAX_CONTRAST 63

#define DISON     0xAF          // Display on
#define DISOFF    0xAE          // Display off
#define DISNOR    0xA6          // Normal display
#define DISINV    0xA7          // Inverse display
#define COMSCN    0xBB          // Common scan direction
#define DISCTL    0xCA          // Display control
#define SLPIN     0x95          // Sleep in
#define SLPOUT    0x94          // Sleep out
#define PASET     0x75          // Page address set
#define CASET     0x15          // Column address set
#define DATCTL    0xBC          // Data scan direction, etc.
#define RGBSET8   0xCE          // 256-color position set
#define RAMWR     0x5C          // Writing to memory
#define RAMRD     0x5D          // Reading from memory
#define PTLIN     0xA8          // Partial display in
#define PTLOUT    0xA9          // Partial display out
#define RMWIN     0xE0          // Read and modify write
#define RMWOUT    0xEE          // End
#define ASCSET    0xAA          // Area scroll set
#define SCSTART   0xAB          // Scroll start set
#define OSCON     0xD1          // Internal oscillation on
#define OSCOFF    0xD2          // Internal oscillation off
#define PWRCTR    0x20          // Power control
#define VOLCTR    0x81          // Electronic volume control
#define VOLUP     0xD6          // Increment electronic control by 1
#define VOLDOWN   0xD7          // Decrement electronic control by 1
#define TMPGRD    0x82          // Temperature gradient set
#define EPCTIN    0xCD          // Control EEPROM
#define EPCOUT    0xCC          // Cancel EEPROM control
#define EPMWR     0xFC          // Write into EEPROM
#define EPMRD     0xFD          // Read from EEPROM
#define EPSRRD1   0x7C          // Read register 1
#define EPSRRD2   0x7D          // Read register 2
#define NOP       0x25          // NOP instruction

static DEV_spi_dev_t *spi;

// TODO spi bus and device
static void
write_command (ACE_u32_t cmd)
{
    DEV_spi_exchange (spi, (cmd & ~0x0100), NULL);      // BIT9 = 0 -> command
}

static void
write_data (ACE_u32_t data)
{
    DEV_spi_exchange (spi, (data | 0x0100), NULL);      // BIT9 = 1 -> data
}

static void
set_column_address (int x, int y)
{
    // column address set
    write_command (CASET);
    write_data (x);
    write_data (y);
}

static void
set_page_address (int x, int y)
{
    // page address set
    write_command (PASET);
    write_data (x);
    write_data (y);
}

extern int
DPY_nokia6100_spi_init (DEV_spi_dev_t * dev)
{
    spi = dev;

    if (0 != DEV_spi_dev_config (spi, 0x10,     /* clock */
                                 3,     /* mode */
                                 9,     /* word_size */
                                 1,     /* delay_b_sclk */
                                 1,     /* delay_b_ct */
                                 DEV_SPI_DEV_CS_PASSIVE))
        return (-1);
    return 0;
}

// TODO initialization
// -> layout setting (mirror x,y)
// -> inversion
// -> color mode (8bit, 12bit)
extern void
DPY_nokia6100_init (DEV_spi_dev_t * dev)
{
    DEV_spi_acquire (spi);
    // Display control
    write_command (DISCTL);
    write_data (0x00);          // P1: 0x00 = 2 divisions, switching period=8 (default)
    write_data (0x20);          // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
    write_data (0x00);          // P3: 0x00 = no inversely highlighted lines
    // COM scan
    write_command (COMSCN);
    write_data (1);             // P1: 0x01 = Scan 1->80, 160<-81
    // Internal oscilator ON
    write_command (OSCON);
    // Sleep out
    write_command (SLPOUT);
    // Power control
    write_command (PWRCTR);
    write_data (0x0f);          // reference voltage regulator on, circuit voltage follower on, BOOST ON
    // Inverse display
    write_command (DISINV);
    // Data control
    write_command (DATCTL);
    write_data (0x01);          // P1: 0x01 = page address inverted, column address normal, address scan in column direction
    write_data (0x00);          // P2: 0x00 = RGB sequence (default value)
    write_data (0x02);          // P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A)
    // Voltage control (contrast setting)
    write_command (VOLCTR);
    write_data (32);            // P1 = 32 volume value (experiment with this value to get the best contrast)
    write_data (3);             // P2 = 3    resistance ratio (only value that works)
    // allow power supply to stabilize
    DEV_spi_release (spi);
    USO_sleep (USO_MSEC_2_TICKS (50));
    // turn on the display
    DEV_spi_acquire (spi);
    write_command (DISON);
    DEV_spi_release (spi);
}

extern void
DPY_nokia6100_backlight (DEV_digout_t * backlight, ACE_bool_t state)
{
    if (TRUE == state)
        DEV_digout_set (backlight);
    else
        DEV_digout_clear (backlight);
}

extern void
DPY_nokia6100_reset (DEV_digout_t * reset)
{
    // Hardware reset
    DEV_digout_clear (reset);
    USO_sleep (USO_MSEC_2_TICKS (10));
    DEV_digout_set (reset);
    USO_sleep (USO_MSEC_2_TICKS (10));
}

extern void
DPY_nokia6100_write_bitmap (const unsigned char bmp[], int x1, int x2, int y1, int y2, int size)
{
    int i;

    DEV_spi_acquire (spi);
    // Data control (need to set "normal" page address for Olimex photograph)
    write_command (DATCTL);
    write_data (0x00);          // P1: 0x00 = page address normal, column address normal, address scan in column direction
    write_data (0x00);          // P2: 0x00 = RGB sequence (default value)
    write_data (0x02);          // P3: 0x02 = Grayscale -> 16
    // Display OFF
    write_command (DISOFF);

    // set area
    set_column_address (x1, x2);
    set_page_address (y1, y2);

    // write memory
    write_command (RAMWR);
    for (i = 0; i < size; i++)
    {
        write_data (bmp[i]);
    }
    // terminate the Write Memory command
    write_command (NOP);
    // Data control (return to  "inverted" page address)
    write_command (DATCTL);
    write_data (0x01);          // P1:  0x01 = page address inverted, column address normal, address scan in column direction
    write_data (0x00);          // P2:  0x00 = RGB sequence (default value)
    write_data (0x02);          // P3:  0x02 = Grayscale -> 16
    // Display On
    write_command (DISON);
    DEV_spi_release (spi);
}

extern void
DPY_nokia6100_clear_screen (void)
{
    int i;

    DEV_spi_acquire (spi);
    // set area
    set_column_address (0, 131);
    set_page_address (0, 131);

    // set the display memory to BLACK
    write_command (RAMWR);
    for (i = 0; i < ((131 * 131) / 2); i++)
    {
        write_data ((DPY_NOKIA6100_BLACK >> 4) & 0xFF);
        write_data (((DPY_NOKIA6100_BLACK & 0xF) << 4) | ((DPY_NOKIA6100_BLACK >> 8) & 0xF));
        write_data (DPY_NOKIA6100_BLACK & 0xFF);
    }
    DEV_spi_release (spi);
}

extern void
DPY_nokia6100_set_contrast (unsigned int contrast)
{
    if (contrast > MAX_CONTRAST)
        return;

    DEV_spi_acquire (spi);
    // voltage control (contrast setting)
    write_command (VOLCTR);
    write_data (contrast);      // P1 = 32 volume value (experiment with this value to get the best contrast)
    write_data (3);             // P2 = 3    resistance ratio (only value that works)
    DEV_spi_release (spi);
}

static void
nokia6100_set_pixel (int x, int y, int color)
{
    // set area
    set_page_address (x, x);
    set_column_address (y, y);

    // now illuminate the pixel (2nd pixel will be ignored)
    write_command (RAMWR);
    write_data ((color >> 4) & 0xFF);
    write_data (((color & 0xF) << 4) | ((color >> 8) & 0xF));
    write_data (color & 0xFF);
}

extern void
DPY_nokia6100_set_pixel (int x, int y, int color)
{
    DEV_spi_acquire (spi);
    nokia6100_set_pixel (x, y, color);
    DEV_spi_release (spi);
}

static void
nokia6100_set_line (int x1, int y1, int x2, int y2, int color)
{
    int dy = y2 - y1;
    int dx = x2 - x1;
    int stepx, stepy;

    if (dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }
    if (dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }
    dy <<= 1;                   // dy is now 2*dy
    dx <<= 1;                   // dx is now 2*dx

    nokia6100_set_pixel (x1, y1, color);
    if (dx > dy)
    {
        int fraction = dy - (dx >> 1);  // same as 2*dy - dx

        while (x1 != x2)
        {
            if (fraction >= 0)
            {
                y1 += stepy;
                fraction -= dx; // same as fraction -= 2*dx
            }
            x1 += stepx;
            fraction += dy;     // same as fraction -= 2*dy
            nokia6100_set_pixel (x1, y1, color);
        }
    }
    else
    {
        int fraction = dx - (dy >> 1);
        while (y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            nokia6100_set_pixel (x1, y1, color);
        }
    }
}

extern void
DPY_nokia6100_set_line (int x1, int y1, int x2, int y2, int color)
{
    DEV_spi_acquire (spi);
    nokia6100_set_line (x1, y1, x2, y2, color);
    DEV_spi_release (spi);
}

static void
nokia6100_set_rectangle (int x1, int y1, int x2, int y2, unsigned char fill, int color)
{
    int xmin, xmax, ymin, ymax;
    int i;

    // check if the rectangle is to be filled
    if (fill == DPY_NOKIA6100_FILL)
    {
        // best way to create a filled rectangle is to define a drawing box
        // and loop two pixels at a time
        // calculate the min and max for x and y directions
        xmin = (x1 <= x2) ? x1 : x2;
        xmax = (x1 > x2) ? x1 : x2;
        ymin = (y1 <= y2) ? y1 : y2;
        ymax = (y1 > y2) ? y1 : y2;
        // specify the controller drawing box according to those limits

        // set area
        set_page_address (xmin, xmax);
        set_column_address (ymin, ymax);

        // write memory
        write_command (RAMWR);
        // loop on total number of pixels / 2
        for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 130); i++)
        {
            // use the color value to output three data bytes covering two pixels
            write_data ((color >> 4) & 0xFF);
            write_data (((color & 0xF) << 4) | ((color >> 8) & 0xF));
            write_data (color & 0xFF);
        }
    }
    else
    {
        // best way to draw un  unfilled rectangle is to draw four lines
        nokia6100_set_line (x1, y1, x2, y1, color);
        nokia6100_set_line (x1, y2, x2, y2, color);
        nokia6100_set_line (x1, y1, x1, y2, color);
        nokia6100_set_line (x2, y1, x2, y2, color);
    }
}

extern void
DPY_nokia6100_set_rectangle (int x1, int y1, int x2, int y2, unsigned char fill, int color)
{
    DEV_spi_acquire (spi);
    nokia6100_set_rectangle (x1, y1, x2, y2, fill, color);
    DEV_spi_release (spi);
}

static void
nokia6100_set_circle (int x, int y, int radius, int color)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int dx = 0;
    int dy = radius;

    nokia6100_set_pixel (x, y + radius, color);
    nokia6100_set_pixel (x, y - radius, color);
    nokia6100_set_pixel (x + radius, y, color);
    nokia6100_set_pixel (x - radius, y, color);
    while (dx < dy)
    {
        if (f >= 0)
        {
            dy--;
            ddF_y += 2;
            f += ddF_y;
        }
        dx++;
        ddF_x += 2;
        f += ddF_x + 1;
        nokia6100_set_pixel (x + dx, y + dy, color);
        nokia6100_set_pixel (x - dx, y + dy, color);
        nokia6100_set_pixel (x + dx, y - dy, color);
        nokia6100_set_pixel (x - dx, y - dy, color);
        nokia6100_set_pixel (x + dy, y + dx, color);
        nokia6100_set_pixel (x - dy, y + dx, color);
        nokia6100_set_pixel (x + dy, y - dx, color);
        nokia6100_set_pixel (x - dy, y - dx, color);
    }
}

extern void
DPY_nokia6100_set_circle (int x, int y, int radius, int color)
{
    DEV_spi_acquire (spi);
    nokia6100_set_circle (x, y, radius, color);
    DEV_spi_release (spi);
}

static void
nokia6100_put_char (char c, int x, int y, int size, int fcolor, int bcolor)
{
    extern const unsigned char FONT6x8[97][8];
    extern const unsigned char FONT8x8[97][8];
    extern const unsigned char FONT8x16[97][16];
    int i, j;
    unsigned int nCols;
    unsigned int nRows;
    unsigned int nBytes;
    unsigned char PixelRow;
    unsigned char Mask;
    unsigned int Word0;
    unsigned int Word1;
    unsigned char *pFont;
    unsigned char *pChar;
    unsigned char *FontTable[] =
        { (unsigned char *)FONT6x8, (unsigned char *)FONT8x8, (unsigned char *)FONT8x16 };

    // get pointer to the beginning of the selected font table
    pFont = (unsigned char *)FontTable[size];
    // get the nColumns, nRows and nBytes
    nCols = *pFont;
    nRows = *(pFont + 1);
    nBytes = *(pFont + 2);
    // get pointer to the last byte of the desired character
    pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;

    // set area
    set_page_address (x, x + nRows - 1);
    set_column_address (y, y + nCols - 1);

    // write memory
    write_command (RAMWR);
    // loop on each row, working backwards from the bottom to the top
    for (i = nRows - 1; i >= 0; i--)
    {
        // copy pixel row from font table and then decrement row
        PixelRow = *pChar--;
        // loop on each pixel in the row (left to right)
        // Note: we do two pixels each loop
        Mask = 0x80;
        for (j = 0; j < nCols; j += 2)
        {
            // if pixel bit set, use foreground color; else use the background color
            // now get the pixel color for two successive pixels
            if ((PixelRow & Mask) == 0)
                Word0 = bcolor;
            else
                Word0 = fcolor;
            Mask = Mask >> 1;
            if ((PixelRow & Mask) == 0)
                Word1 = bcolor;
            else
                Word1 = fcolor;
            Mask = Mask >> 1;
            // use this information to output three data bytes
            write_data ((Word0 >> 4) & 0xFF);
            write_data (((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
            write_data (Word1 & 0xFF);
        }
    }
    // terminate the Write Memory command
    write_command (NOP);
}

extern void
DPY_nokia6100_put_char (char c, int x, int y, int size, int fcolor, int bcolor)
{
    DEV_spi_acquire (spi);
    nokia6100_put_char (c, x, y, size, fcolor, bcolor);
    DEV_spi_release (spi);
}

static void
nokia6100_put_string (char *string, int x, int y, int size, int fcolor, int bcolor)
{
    // loop until null-terminator is seen
    while (*string != 0x00)
    {
        // draw the character
        nokia6100_put_char (*string++, x, y, size, fcolor, bcolor);
        // advance the y position
        if (size == DPY_NOKIA6100_SMALL)
            y = y + 6;
        else if (size == DPY_NOKIA6100_MEDIUM)
            y = y + 8;
        else
            y = y + 8;
        // bail out if y exceeds 131
        if (y > 131)
            break;
    }
}

extern void
DPY_nokia6100_put_string (char *string, int x, int y, int size, int fcolor, int bcolor)
{
    DEV_spi_acquire (spi);
    nokia6100_put_string (string, x, y, size, fcolor, bcolor);
    DEV_spi_release (spi);
}
