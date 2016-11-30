# snowflake-2016
An ATtiny based snowflake ornament. Happy Holidays friends...

### Board shape
  * created the board in Illustrator and added extra points (`Object -> Path -> Add Anchor Points`).
  * exported shape as `DXF` - settings:
    * R13/LT95
    * scale - 1mm = 1 unit
    * colors 256 - PNG
    * max editabitity
    * export selected art only
  * import shape using `import_dxf_polygons_v4.ulp` ([found here](https://raw.githubusercontent.com/todbot/eagle-tricks/master/import_dxf_polygons_v4.ulp)) - settings:
    * scale 1
    * import to layer: 20 - dimension
    * wire or polygon: wire
    * pen width: .2
  * make sure shape is 'watertight' - had to clean up small space on snowflake in Eagle by drawing a 'wire' to close.