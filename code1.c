#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_DEVICE "/dev/i2c-1"  // I2C device file
#define SSD1306_ADDR 0x3C        // SSD1306 display address

int i2c_fd;

// Function to send commands to SSD1306
void ssd1306_command(unsigned char command) {
    unsigned char buffer[2];
    buffer[0] = 0x00;   // Co = 0, D/C# = 0 (command)
    buffer[1] = command;
    write(i2c_fd, buffer, 2);
}

// Function to send data to SSD1306
void ssd1306_data(unsigned char data) {
    unsigned char buffer[2];
    buffer[0] = 0x40;   // Co = 0, D/C# = 1 (data)
    buffer[1] = data;
    write(i2c_fd, buffer, 2);
}

// Function to initialize SSD1306
void ssd1306_init() {
    ssd1306_command(0xAE);  // Display off
    
    // ... Add more initialization commands as needed
    
    ssd1306_command(0xAF);  // Display on
}

// Function to clear the display
void ssd1306_clear() {
    int i, j;
    
    for (i = 0; i < 8; i++) {
        ssd1306_command(0xB0 + i);    // Set page address
        
        for (j = 0; j < 128; j++) {
            ssd1306_data(0x00);        // Clear all columns
        }
    }
}

// Function to display a character at the specified position
void ssd1306_display_char(unsigned char row, unsigned char column, char character) {
    int i;
    column = column * 6;  // Each character is 6 columns wide
    
    ssd1306_command(0xB0 + row);  // Set page address
    ssd1306_command(0x00 + (column & 0x0F));   // Set lower column address
    ssd1306_command(0x10 + ((column >> 4) & 0x0F));  // Set higher column address
    
    for (i = 0; i < 6; i++) {
        ssd1306_data(font[character - 32][i]);   // Display character from font array
    }
}

int main() {
    // Open I2C device file
    i2c_fd = open(I2C_DEVICE, O_RDWR);
    if (i2c_fd < 0) {
        perror("Unable to open I2C device");
        return 1;
    }
    
    // Set I2C slave address for SSD1306
    if (ioctl(i2c_fd, I2C_SLAVE, SSD1306_ADDR) < 0) {
        perror("Unable to set I2C slave address");
        return 1;
    }
    
    // Initialize SSD1306 display
    ssd1306_init();
    
    // Clear the display
    ssd1306_clear();
    
    // Display "Hello, World!" at row 2, column 5
    const char *message = "Hello, World!";
    for(int i=0; i<strlen(message); i++)
    {
        ssd1306_display_char(2, 5+i, message[i]);
    }

    // Close I2C device file
    close(i2c_fd);
    
    return 0;
}
