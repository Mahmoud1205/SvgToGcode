# SvgToGcode

Convert SVG to g-code so that any 3-axis CNC machine can be used as a pen plotter.

```
Usage: SvgToGcode.exe -i [inFileName.svg] -o [outFileName.nc]
'-f' [optional, default: 3000.0] can be used to set the XY feedrate in mm/min.
'-z' [optional, default: -2.0] can be used to set the z depth to move while drawing, in millimeters.
'-d' [optional, default: 96.0] can be used to set the DPI (dots per inch) to parse the input SVG with.
```

Example usage: `SvgToGcode.exe -i drawing.svg -o drawing.nc -f 3000 -z -2 -d 700`
