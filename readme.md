# Hidden Star
Hidden Star is a work-in-progress 2D sci-fi exploration/trading/combat game, initially created initially for [LOWREZJAM 2016](https://itch.io/jam/lowrezjam2016).


## Licence
Hidden Star is licenced under GPLv3, with sources available on [GitHub](https://github.com/Cheeseness/hidden-star). For more details, see the LICENSE file.


## Play Instructions

### Star Map
Navigate the known galaxy by clicking on adjacent stars to travel to them.
All travel nodes are currently revealed. In-travel gameplay is currently unavailable (travel is instantaneous).

### Communications
Select dialogue options to avoid or initiate combat.
Faction disposition will shift slightly based on comms outcomes. Info and trade options are currently unavailable.

### Combat
Queue up two actions during a timed turn in order to avoid enemy fire while dealing damage.
Only one enemy movement/firing pattern is currently available.


## Changes

Below are a summary of notable changes for releases. More detailed commit history can be found on [GitHub](https://github.com/Cheeseness/hidden-star/commits/master).

### 2016-05-14 Build (fae01e4)
* Comms screens now use dynamic planets
* Skipping NPC is now two-stage (once to complete text, again to skip entirely)
* NPC text is now visually translated from faction's native charset
* NPCs now have faction and disposition specific dialogue
* Starmap is now pannable
* Replaced random placeholder starmap with per-faction tree based generation
* Fixed non-64x64px starmap node connection lines
* Stars now have generated star names
* Added screenshot support (F12)
* Combat mode uses global GUI functions
* Factions now have placeholder disposition functionality

### 2016-04-18 End-of-jam Build (6909f87)
* Placeholder main menu
* Placeholder comms screen
* Placeholder starmap screen
* Early combat implementation

