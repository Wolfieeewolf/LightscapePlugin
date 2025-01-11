# How to Use Lightscape

## ⚠️ IMPORTANT WARNING ⚠️

Before getting started, please be aware:
- This plugin is in a very early experimental alpha stage
- Most features are in testing and may break at any time
- You should expect frequent issues including but not limited to:
  - Effects freezing or behaving unexpectedly
  - Grid configurations not saving properly
  - UI becoming unresponsive
  - Devices not responding to commands
  - Color updates lagging or failing
  - Assignments disappearing
  - Crashes and freezes
- Bug fixes take significant time as AI-assisted development often leads to:
  - Fixing one bug creating several others
  - Unexpected interactions between features
  - Inconsistent behavior across different systems
- Save your RGB device configurations before using this plugin
- Use at your own risk and expect things to go wrong - this is normal in this stage of development

## Initial Setup Guide

For the best experience, complete these steps in order:

1. Configure Your Grid First:
   - Click "Configure Grid" immediately after launching
   - Set your desired dimensions (width, height, layers)
   - Customize layer names if desired
   - Click "Apply" to save your grid configuration
   - Features may not work correctly until you configure your grid

2. Set Your User Position:
   - Select a position in your configured grid
   - Click "Set User Position"
   - This position is needed for effects to work properly
   - Device assignments might not work until you set a user position

3. After completing steps 1 and 2 you can:
   - Assign devices to positions
   - Use effects
   - Control colors
   - Make any other changes

## Basic Controls

### Grid Navigation
- Each layer is shown in a tab
- Default layers are labeled "Top", "Middle", and "Bottom"
- Each position in the grid shows its label (P1, P2, etc.)

### Device Assignment
1. Select a position in the grid by clicking it
2. Choose a device from the dropdown menu
3. Select what to assign:
   - Whole Device: Assigns the entire device
   - Zone: Assigns a specific zone of the device
   - LED: Assigns a single LED
4. Click "Assign" to map the selection to the grid position

### Effects System
1. Choose an effect from the dropdown:
   - None: Disables effects
   - RadialFade: Brightness based on distance from user
   - Wave: Rippling wave pattern from user position
   - Ripple: Expanding ring effect from user position
   - LayerCascade: Cascading effect across layers

2. Adjust effect controls:
   - Speed: Controls how fast the effect moves
   - Intensity: Controls the strength of the effect

### Color Control
1. Select a grid position with assignments
2. Select an assignment from the list
3. Click "Set Color" to choose a color
4. The color will be applied immediately

## Customizing the Grid

### Configure Grid Dimensions
1. Click "Configure Grid"
2. Adjust dimensions:
   - Width: 1-10 positions
   - Height: 1-10 positions
   - Layers: 1-5 layers
3. Click "Apply" to save changes

### Customizing Labels
1. In the grid configuration:
   - Change layer names by editing the "Layer Name" field
   - Edit position labels in the table
2. Click "Apply" to save changes
3. Use "Reset to Defaults" to restore default labels

### Managing Assignments
- "Remove": Removes selected assignment
- "Clear": Removes all assignments from position

## Tips & Tricks

1. Plan Your Layout
   - Sketch your physical device layout first
   - Consider using layers for different heights
   - Use meaningful position labels

2. Effect Usage
   - Start with low intensity to test effects
   - Adjust speed gradually for best results

3. Color Management
   - Test colors with a single position first
   - Use the color picker's recent colors