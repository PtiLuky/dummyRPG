# File format

### Game definition file

Extension **.gdummy** (g for Game).

*(note : 1B = 1 byte)*

Header: 

```
- File signature: 00 ca 3e 00 (hex)

- Tags followed by values: 
    - TAG_NAME 				(str) 		// 4B = size = N, N B = content
    - TAG_VERSION 			(uint64) 	// 4B = major v, 2B = minor, 2B = patch
	
    - TAG_MAP_COUNT 			(uint16)
    - TAG_ITEM_COUNT			(uint16)
    - TAG_CHARACTER_COUNT		(uint32)
    - TAG_MONSTER_COUNT			(uint32)
    - TAG_EVENT_COUNT			(uint32)
	- TAG_CHIPSET_COUNT			(uint8)
	- TAG_DYNAMIC_SPRITE_COUNT	(uint16)
```

Content:  
The order or different tags is not mandatory.

* 2B = **TAG_CHIPSETS**. Then *chipsets count* times:
	* 2B = TAG_CHIPSET
	* **chipset path** 		[4B = path size = nameSize] + [nameSize B = path]

* 2B = **TAG_MAPS**. Then *maps count* times:
	* 2B = TAG_MAP
	* **chipset count** 	[1B = chipCnt] + chipCnt x [1B = chip id]
	* **width/height** 		[2B = width] + [2B = height]
	* **floor count** 		[1B = floorCnt]. Then *floorCnt* times:
		* 2B = TAG_FLOOR
		* **blocking layer** 	[width*height B = blocking data]
		* 1B = graphic layers count = graphLayerCount. Then *graphLayerCount* times:
			* [width x height x 3 B = graphic data]
				
* 2B = **TAG_ITEMS**. Then *items count* times:
	* 2B = TAG_ITEM
	* **name** 		[4B = item name size = nameSize] + [nameSize B = item name]
	* **sprite** 	[2B = sprite id]
				
* 2B = **TAG_CHARACTERS**. Then *characters count* times:
	* 2B = TAG_CHARACTER
	* **name** 		[4B = char name size = nameSize] + [nameSize B = character name]
	* **sprite** 	[2B = sprite id]
				
* 2B = **TAG_MONSTERS**. Then *monsters count* times:
	* 2B = TAG_MONSTER
	* **name** 			[4B = char name size = nameSize] + [nameSize B = character name]
	* **sprite** 		[2B = sprite id]
	* **attack curve** 	[5 x 1B = curve]
	* **defense curve** [5 x 1B = curve]
	* **health curve** 	[5 x 1B = curve]
	
* 2B = **TAG_DYNAMIC_SPRITES**. Then *sprites count* times:
	* 2B = TAG_DYNAMIC_SPRITE
	* **sprite path** 	[4B = sprite path size = nameSize] + [nameSize B = sprite path]
	* **x/y** 			[2B = x] + [2B = y]
	* **width/height** 	[2B = width] + [2B = height]
	* **frames count anim1** 	[1B = frames count]
	* **animation2 xy + count**	[2B = x] + [2B = y] + [1B = frames count]
	* **animation3 xy + count**	[2B = x] + [2B = y] + [1B = frames count]
	* **animation3 xy + count**	[2B = x] + [2B = y] + [1B = frames count]
	
End:
```
- File signature: 00 ca 3e 00 (hex)
```