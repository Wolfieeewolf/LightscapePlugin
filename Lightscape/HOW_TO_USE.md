# How to Use Lightscape

## Getting Started

1. Launch OpenRGB
2. Look for the "Lightscape" tab in the main window
3. The default grid is 3x3 with 3 layers (Top, Middle, Bottom)

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

2. Efficient Assignment
   - Assign whole devices when possible
   - Use zones for strips or larger areas
   - Individual LEDs for precise control

3. Color Management
   - Test colors with a single position first
   - Use the color picker's recent colors
   - Check effects in real-time

## Troubleshooting

1. Grid Not Updating
   - Check if OpenRGB detects your devices
   - Try reassigning devices
   - Restart OpenRGB

2. Missing Assignments
   - Verify device is still connected
   - Check if dimensions were reduced
   - Reassign if needed

3. Color Issues
   - Ensure device is properly connected
   - Check device brightness settings
   - Verify color support in device

## Additional Notes

- Changes are applied in real-time
- Assignments persist between sessions
- Grid configurations are saved automatically
- Maximum grid size depends on available space
- Not all devices support all features