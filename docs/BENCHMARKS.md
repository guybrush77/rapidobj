# Benchmarks

This is a comparison of the time it takes to load and parse various .obj files. The libraries compared are:
* OBJ-Loader - https://github.com/Bly7/OBJ-Loader
* rapidobj - https://github.com/guybrush77/rapidobj
* tinyobjloader - https://github.com/tinyobjloader/tinyobjloader

#### Test Hardware

Acer Swift 3 Laptop (_AMD Ryzen 7 4700U, 8GB 3200MHz LPDDR4, 
512GB NVMe SSD_)

#### Operating System
Ubuntu 21.04

#### Compiler
g++ 10 (_-O3 -DNDEBUG -std=gnu++17_)

## Results

File: fireplace_room.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh13
Size on disk: 27,955,115 bytes
Triangles: 143,173

![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-fireplace_room.svg)

---
File: hairball.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh16
Size on disk: 236,055,639 bytes
Triangles: 2,880,000
![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-hairball.svg)

---
File: powerplant.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh25
Size on disk: 817,891,724 bytes
Triangles: 12,759,246
![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-powerplant.svg)

---
File: roadBike.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh26
Size on disk: 143,552,387 bytes
Triangles: 1,677,520
![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-roadBike.svg)

---
File: rungholt.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh27
Size on disk: 275,675,419 bytes
Triangles: 6,704,264
![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-rungholt.svg)

---
File: salle_de_bain.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh28
Size on disk: 123,949,346 bytes
Triangles: 1,231,030

![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-salle_de_bain.svg)

---
File: san-miguel.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh29
Size on disk: 1,143,041,382 bytes
Triangles: 9,980,699
![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-san-miguel.svg)

---
File: sponza.obj
URL: https://casual-effects.com/g3d/data10/index.html#mesh32
Size on disk: 5,610,593 bytes
Triangles: 66,450
![bench-fireplace_room](https://raw.githubusercontent.com/guybrush77/rapidobj/master/data/images/bench-sponza.svg)