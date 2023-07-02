 

```forth
variable i2c-fd
variable SSD1306-ADDR 0x3C constant 

: ssd1306-command ( command -- )
  0x00 2array 
  swap ! 
  i2c-fd @ write ;

: ssd1306-data ( data -- )
  0x40 2array 
  swap ! 
  i2c-fd @ write ;

: ssd1306-init
  0xAE ssd1306-command 
  0xAF ssd1306-command ;

: ssd1306-clear
  0 8 0 DO
    I 0xB0 + ssd1306-command
    0 128 0 DO
      0 ssd1306-data 
    LOOP
  LOOP ;

: ssd1306-display-char ( row column character -- )
  6 * >r 
  0xB0 r> + ssd1306-command
  r@ 0x0F and 0x00 + ssd1306-command
  r> 4 rshift 0x0F and 0x10 + ssd1306-command 
  0 6 0 DO
    font I - 32 + @ ssd1306-data 
  LOOP ;

: display-message ( addr len -- )
  2 5 2>r
  DO
    r> r@ ssd1306-display-char
    r> 1+ 2>r
  LOOP
  2rdrop ;

: main
  "/dev/i2c-1" w/o open-file throw to i2c-fd 
  SSD1306-ADDR i2c-fd @ set-slave-address
  ssd1306-init 
  ssd1306-clear 
  s" Hello, World!" display-message 
  i2c-fd @ close-file throw ;
```
 
