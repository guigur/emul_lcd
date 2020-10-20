# emul_lcd

![LCD demo](IMGS/lcd_blinking_cursor.gif?raw=true "LCD demo")

### What is it ?
This library display a virtual 16x2 LCD on your computer !
It's written in C++ and is (hopefully) cross-platform
For now, only a debug Visualstudio solution in present in the project but this will change.
This library will be near 100% compatible calls with the regular LiquidCrystal one.
It can be very useful if you're working on your PC and don't have access to actual hardware.

### 100% compatible ?
Well, keep in mind, this is still **Work in Progress**, so not all calls are implemented yet.

| Method                 | status | Commentary |
| :--------------------- | :----: | :--------- |
| `LiquidCrystal()`      | ✔️ | Needs cleanup |
| `begin()`              | ✔️ |               |
| `clear()`              | ✔️ |               |
| `home()`               | ✔️ |               |
| `setCursor()`          | ✔️ |               |
| `write()`              | ✔️ |               |
| `print()`              | ✔️ |               |
| `cursor()`             | ✔️ |               |
| `noCursor()`           | ✔️ |               |
| `blink()`              | ✔️ |               |
| `noBlink()`            | ✔️ |               |
| `display()`            | ✔️ |               |
| `noDisplay()`          | ✔️ |               |
| `scrollDisplayLeft()`  | ❌ | TODO          |
| `scrollDisplayRight()` | ❌ | TODO          |
| `autoscroll()`         | ❌ | TODO          |
| `noAutoscroll()`       | ❌ | TODO          |
| `leftToRight()`        | ❌ | TODO          |
| `rightToLeft()`        | ❌ | TODO          |
| `createChar()`         | ✔️ |               |

### The charset
Here is the implemented charset of this program:

![charset](https://raw.githubusercontent.com/guigur/display_charset/main/gen%20charset%20bitmap/gen%20charset%20bitmap/charset%20LCD%20big.png "LCD demo")

(Note: This image has been generated with the [display charset](https://github.com/guigur/display_charset) program)
