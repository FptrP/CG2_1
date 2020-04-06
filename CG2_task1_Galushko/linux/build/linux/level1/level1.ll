map {
    _filename = "level1/map.png"
}

texture {
    _name = "start_chest_tex"
    _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_261.png"
}

texture {
    _name = "chest_tex"
    _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_262.png"
}

texture {
    _name = "tree_tex"
    _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_278.png"
}

texture {
    _name = "stone_tex"
    _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_1085.png"
}

texture {
    _name = "wall_tex"
    _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_345.png"
}

tile {
    _name = "stone_tile"
    _texture = "stone_tex"
}

tile {
    _name = "tree_tile"
    _texture = "tree_tex"
}

tile {
    _name = "wall_tile"
    _texture = "wall_tex"
}

entity {
  _type = "Walls"
  _tile = "wall_tile"
  _patterned = true
  _colorBinding = #FFFFFF
}

entity {
  _type = "Walls"
  _tile = "tree_tile"
  _patterned = true
  _colorBinding = #00DB00
}

entity {
  _type = "Walls"
  _tile = "stone_tile"
  _patterned = true
  _colorBinding = #6D6D6D
}