# Lightscape Plugin for OpenRGB

Lightscape is a 3D spatial LED control system plugin for OpenRGB that allows you to organize and control your RGB devices in a three-dimensional grid layout.

## Project Background

This project was inspired by the Philips amBX Gaming peripheral system from 2006. The goal is to recreate and modernize the amBX concept using current RGB hardware and the OpenRGB framework. For reference, you can see the original amBX system here: [Philips amBX Gaming Peripherals](https://www.youtube.com/watch?v=TWC3ruGYif0)

## Development Note

This entire plugin was created using Claude AI from Anthropic. It's an experiment to explore what's possible with AI-assisted development. As such:

- This is a very early alpha version
- There may be bugs and issues
- Some features might not work as expected
- Error handling might be incomplete

## Planned Features

Future development plans include:
- Spatial effects (like the original amBX)
- Game integration
- Screen capture and ambient lighting
- Enhanced 3D positioning system
- More customization options

However, as this is an AI-assisted project, implementing these features will depend on successfully working with AI to add them without breaking existing functionality.

## Open Source and Collaboration

Feel free to fork this repository and modify it however you like! This project is meant to be:
- An experiment in AI-assisted development
- A learning resource for others
- A foundation for community improvements
- Open to any and all modifications

If you're interested in the intersection of AI and development, this project serves as a real-world example of what's possible.

## Features

- 3D Grid Layout: Organize your RGB devices in a customizable 3-dimensional grid
- Multiple Layers: Support for up to 5 vertical layers
- Customizable Grid: Adjustable grid dimensions (up to 10x10 per layer)
- Custom Labels: Rename positions and layers for better organization
- Flexible Control: Assign whole devices, zones, or individual LEDs to grid positions
- Color Management: Easy color control for assigned devices
- Real-time Updates: See your changes instantly

## Installation

1. Download the plugin file (Lightscape.dll)
2. Place it in your OpenRGB plugins folder:
   - Windows: `%APPDATA%\OpenRGB\plugins`
   - Linux: `~/.config/OpenRGB/plugins`
   - MacOS: `~/Library/Application Support/OpenRGB/plugins`
3. Restart OpenRGB

## Build Instructions

### Prerequisites
- Qt 5.15 or newer
- OpenRGB source code
- C++ compiler with C++17 support

### Building
1. Clone or copy the plugin directory into OpenRGB's `plugins` folder
2. Open the project in Qt Creator
3. Configure the project with your Qt kit
4. Build the project

### Manual Build
```bash
cd OpenRGB/plugins/Lightscape
qmake
make
```

## Development Status

This plugin is in very early alpha stage. Expect bugs and incomplete features. Please report any issues or feature requests through GitHub issues, but understand that as this is an AI-assisted project, fixes and updates may take time as we experiment with the best ways to implement changes.

## Acknowledgments

- Built with assistance from Claude AI (Anthropic)
- Inspired by Philips amBX Gaming Peripherals
- Made possible by the OpenRGB framework

## License

This project is open-source software. Feel free to experiment, modify, and improve upon it!