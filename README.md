# BitView

Application for visual analysis of binary files.

## Project Features

**BitView** is a specialized application for binary file analysis, developed on the Qt framework. The main feature of the program is a unique data visualization method - "mattress" unfolding, which allows:

- **Visualize bit stream** as a colored "mattress" with periodic unfolding
- **Analyze patterns** in binary data through color coding of bits (0/1)
- **Scale the display** for detailed study of individual sections
- **Demultiplex channels** from the data stream into separate files

### Main Features

- **Two display modes:**
  - Bit mode (Bit View) - each bit is displayed as a separate pixel
  - Hexadecimal mode (Hex View) - data is grouped by 4 bits

- **Data processing:**
  - Bit shifting for analysis from different positions
  - Byte reversal (bit order inversion)
  - Channel demultiplexing


## MVC Architecture

The project is implemented using the **Model-View-Controller (MVC)** pattern with an additional delegate layer for a flexible display system.

### Model
```cpp
class Model : public QObject
```
**Responsibilities:**
- File data access management
- Data caching (reading in 16MB pages)
- Bit shifting and byte reversal processing
- Channel demultiplexing

**Key methods:**
- `getBits()` - getting an array of bits for display
- `setShift()` - setting bit shift
- `setReverse()` - enabling/disabling reversal
- `demultiplexChannel()` - extracting a channel to a separate file

### Viewer
```cpp
class Viewer : public QWidget
```
**Responsibilities:**
- Displaying data as a "mattress"
- Handling user input (mouse, wheel)
- Managing scrolling and scaling

**Features:**
- Custom drawing using QPainter
- Drag-and-drop support for navigation
- Automatic updates when data changes

### Delegate (Display Delegate)
```cpp
class BaseDelegate : public QObject
```
**Delegate hierarchy:**
- `BaseDelegate` - abstract base class
- `BitDelegate` - delegate for bit display
- `HexDelegate` - delegate for hexadecimal display

**Responsibilities:**
- Defining colors for displaying bits/symbols
- Calculating display element sizes
- Drawing individual data rows

## Building the Project

### Requirements
- **Qt Framework** 5.x or higher
- **C++11 compiler** (MinGW, MSVC, GCC)
- **Qt Creator** (recommended) or qmake

### Build Commands

1. **Open project in Qt Creator:**
   ```
   Open BitView.pro file
   ```

2. **Build via qmake:**
   ```bash
   qmake BitView.pro
   make
   ```

## Usage Example
### Main Application Window
![Main Window](./img/test1.png)
### Demultiplexing Selected Channel
![Demultiplexing Selected Channel](./img/test2.png)
### Using CoolEdit for Processing Selected Information
![](./img/test3.png)
![](./img/test4.png)
![](./img/test5.png)