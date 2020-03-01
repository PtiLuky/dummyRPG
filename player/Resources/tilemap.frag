#version 130

uniform sampler2D tilemap;
uniform vec2 tilemapSize;

uniform sampler2D chip0Tiles;
uniform vec2 chip0PxSize;

uniform sampler2D chip1Tiles;
uniform vec2 chip1PxSize;

uniform sampler2D chip2Tiles;
uniform vec2 chip2PxSize;

uniform sampler2D chip3Tiles;
uniform vec2 chip3PxSize;

uniform int chipCount;

void main()
{
  vec2 tilePos=gl_TexCoord[0].xy / tilemapSize * 16;


  // Map is encoded as following : r=chip_x ; g=chip_y ; b=chip_id (; a=255, ignore)
  float chipX  = floor(texture2D(tilemap,tilePos).r * 255) * 16;
  float chipY  = floor(texture2D(tilemap,tilePos).g * 255) * 16;
  float chipId = floor(texture2D(tilemap,tilePos).b * 255);

  // check out of map
  if (tilePos.x > 1 || tilePos.y > 1 || tilePos.x < 0 || tilePos.y < 0 || chipX == 255 || chipY == 255)
      discard;

  vec2 chipTile = vec2(chipX, chipY);
  vec2 chipSize = chip0PxSize;
  if (chipId == 1)
      chipSize = chip1PxSize;
  if (chipId == 2)
      chipSize = chip2PxSize;
  if (chipId == 3)
      chipSize = chip3PxSize;

  // check out of chipset
  if (chipId >= chipCount || chipX >= chipSize.x || chipY >= chipSize.y)
      discard;

  vec2 tileChipPos = chipTile / chipSize;
  vec2 tileOffsetInPercent = mod(gl_TexCoord[0].xy * 16, 1);
  vec2 tileOffset = tileOffsetInPercent / chipSize * 16;

  if (chipId == 0)
      gl_FragColor = texture2D(chip0Tiles, tileChipPos + tileOffset);
  if (chipId == 1)
      gl_FragColor = texture2D(chip1Tiles, tileChipPos + tileOffset);
  if (chipId == 2)
      gl_FragColor = texture2D(chip2Tiles, tileChipPos + tileOffset);
  if (chipId == 3)
      gl_FragColor = texture2D(chip3Tiles, tileChipPos + tileOffset);
}

