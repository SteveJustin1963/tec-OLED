# tec-OLED


- SSD1306
- Quazar Graphic OLED - Retro Programming in C and Z80 Assembly on the RC2014



![image](https://github.com/SteveJustin1963/tec-OLED/assets/58069246/bdae94a7-a380-424b-a1cd-eeffd68ddb3f)
![image](https://github.com/SteveJustin1963/tec-OLED/assets/58069246/e328bc79-390a-454e-aea9-ccf6808700f6)
![image](https://github.com/SteveJustin1963/tec-OLED/assets/58069246/3f932ba0-b6fa-4fbe-b6f4-2847f023fbe7)
![image](https://github.com/SteveJustin1963/tec-OLED/assets/58069246/25311d24-f7f3-4d09-bd63-2b572f49ab29)

The SSD1306 is a popular monochrome OLED (Organic Light-Emitting Diode) display controller chip. It is commonly used to drive small displays in various electronic projects. The SSD1306 chip supports I2C (Inter-Integrated Circuit) and SPI (Serial Peripheral Interface) communication protocols, making it versatile and easy to integrate into different systems.

Key features of the SSD1306 chip include:

1. Monochrome Display: The SSD1306 supports monochrome displays with resolutions typically ranging from 128x32 pixels to 128x64 pixels. It can display individual pixels, lines, shapes, and text.

2. OLED Technology: The display panels driven by the SSD1306 utilize OLED technology, which provides several advantages such as high contrast, wide viewing angles, and fast response times. Each pixel on the display emits light independently, allowing for vibrant and sharp visuals.

3. Communication Protocols: The SSD1306 can communicate with a microcontroller or any other device using either the I2C or SPI interface. The I2C protocol requires only a few pins for communication, while the SPI protocol offers faster data transfer rates.

4. Low Power Consumption: OLED displays powered by the SSD1306 chip are known for their low power consumption, especially when compared to traditional LCD displays. They don't require a backlight, as each pixel emits light individually, resulting in power savings.

5. Built-in Controller: The SSD1306 has a built-in display controller, which simplifies the display driving process. It handles tasks such as refreshing the display, managing memory, and displaying graphics. This offloads the computational burden from the microcontroller or host device.

6. Software Libraries: Several software libraries and frameworks exist that provide easy-to-use interfaces for controlling SSD1306-based displays. These libraries often offer functions for drawing shapes, displaying text, and managing graphics resources.

The SSD1306 chip is commonly used in small-scale projects like wearable devices, IoT (Internet of Things) applications, embedded systems, and DIY electronics. With its compact size, low power consumption, and versatile communication options, the SSD1306 is a popular choice for adding graphical display capabilities to various projects.

## code
  example of C code that demonstrates how to use the SSD1306 display with an I2C interface. This code assumes you have the necessary libraries and header files installed for I2C communication and the SSD1306 display.

```c
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

```

Please note that this code is a basic example to get you started. You may need to modify it to suit your specific requirements and the display library you are using. Also, make sure to include the necessary font array or font library to display characters on the screen.

## forth `code1.f`
While converting the whole C program into Forth without using any libraries can be quite complex and lengthy, 
here is a rough simplification of how it could look in Forth, using a similar pattern of function definition and execution. 
This is a high-level pseudo-Forth code and is not meant to be a direct working translation of your C code, 
but it should give you an idea of how you might structure your Forth code. Please note that it may be 
not possible to implement everything from the C code to Forth code without libraries especially due to the differences between two languages.

In Forth, you often directly interface with the hardware or the operating system. 
For I2C communication, you may need to write additional Forth words (Forth's equivalent of functions) to interact with the I2C interface of your specific system, which is beyond the scope of this example. The specifics will depend on the Forth system you're using and the platform you're running it on. For a full, working solution you should consult the documentation of your Forth system and the datasheet of your hardware.

## Ref
- https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
- https://www.youtube.com/watch?v=qtZ0iyk-4Ts&ab_channel=NCOTTechnology
- https://tinusaur.org/tag/ssd1306/
- 




