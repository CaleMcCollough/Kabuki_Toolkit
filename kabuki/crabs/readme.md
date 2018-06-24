```
 /\
( /   @ @    ()   
 \  __| |__  /      ██████╗██████╗  █████╗ ██████╗ ███████╗
  -/   "   \-      ██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔════╝
 /-|       |-\     ██║     ██████╔╝███████║██████╔╝███████╗
/ /-\     /-\ \    ██║     ██╔══██╗██╔══██║██╔══██╗╚════██║
 / /-`---'-\ \     ╚██████╗██║  ██║██║  ██║██████╔╝███████║
  /         \       ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝
```

# Chinese Room Abstract Stack Machine (Crabs) API Overview

The easiest way to get up to speed with how Crabs works is by learning about
how the minor seams are organized.

## Minor Seams

### SEAM_0_0_0

Seam develops the Script Integer-to-ASCII (**itoa**) algorithm, the fastest integer to string algorithm in the world.

#### Seam Modules

* `kabuki`
  * `crabs`

### SEAM_0_0_1

Currently does not exist but in the near future will contain the Grisu-X algorithm with Script itoa algorithm.

#### Seam Modules

* `kabuki`
  * `crabs`

### SEAM_0_0_2

Develops the Universal Text Printer (UTF), Universal Text Scanner (UTS), and Clock submodules.

#### Seam Modules

* `kabuki`
  * `crabs`

### SEAM_0_0_3

Develops the ASCII Data Types.

#### Seam Modules

* `kabuki`
  * `crabs`


### SEAM_0_0_4

Develops the BIn, BOut, Slot, Expr,

#### Seam Modules

* `kabuki`
  * `crabs`

### SEAM 0_0_5

Seam develops the Hardware Abstraction Layer.

#### Seam Modules

* `kabuki`
  * `crabs`
  * `drivers`
  * `features`
  * `hal`
  * `platform`


## Author

* [Cale McCollough](https://calemccollough.github.io)

## License

Copyright 2017-8 (C) [Cale McCollough](mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at http://www.apache.org/licenses/LICENSE-2.0. Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.