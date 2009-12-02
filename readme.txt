Quake Wars: Tactical Assault
v0.3.3
================================
http://qwta.moddb.com/

Contents
1. About
2. Features
3. Installation
4. Known Issues
5. Credits


1. About
================================
QWTA is a straightforward mod in intention; it's intended to tweak the core 
gameplay aspects of Enemy Territory: Quake Wars, to create a more believable,
more realistic experience, that plays more like a serious science fiction 
movie, than an arcade game.

QWTA's secondary objective is to bring more classic Quake 2/4 features to
realization in ETQW, and to embrace new developments that fit within the broad
ETQW/Quake 2/Quake 4/Earth-Stroggos war theme.
Generally, QWTA is assuming ETQW is set before Quake 2, and will attempt to
mix Quake 2 and Quake 4 technology, characters, etc, with that in mind,
in addition to attempting to extrapolate modern technology to ETQW's level.

QWTA takes a lot of its gameplay inspiration from a few random(I'm afraid I 
don't recall the names.) realism mods that have popped up for the 
Call of Duty series of games, and the goal is to get something as an 
end-result that plays similarly to those, but in the futuristic setting of 
the Earth-Stroggos war.

At present, QWTA is intended to function with any base ETQW map, without a 
hitch, and it's hoped that this will continue to be the case through QWTA's 
development: while QWTA would certainly welcome QWTA-intended maps, they're 
not something that's felt should be required.

As a player, the preview video footage that was published before ETQW itself
was released, spoke to me of a gritty, tactical, bloody sort of game, 
involving a lot of thinking and strategy, rather than the run-and-gun-fest 
that the released version of BaseETQW is. To that end, QWTA tries to stay 
true to the perceived feeling behind some of that original footage, and the 
impression it had on a lot of people.

2. Features
================================
* Most QWTA-specific features are optional. You can play BaseETQW using QWTA!
  (Or a strange combination of both.)
* Realistic movement.
* Realistic weapons. (Behave more like their real-world counterparts.)
* Other realism-oriented features...
* Radar no longer detects infantry. (But 3rd eye cameras do!)
* Adaptive class-number limiting.
* Blood effects!
* Death/unconscious screen tinting and fading.
* Mxyzptlk's megatexture autodownload.
* Improved spectator controls.
* Dark Matter Cannon mimics Quake 4-singleplayer Dark Matter Gun.
* Hammer missile behaves more like a tactical nuke.
* Hyperblaster mimics Quake 4-singleplayer Nailgun.
* Any vehicle may be airdropped.
* Persistent ranks can be ignored in favour of campaign ranks.
* Bot support for new features. (Bots will drop vehicles.)
* New vehicle: Jupiter Assault Tank.
* BFG10k. (Jupiter tank's main cannon.)
* Alternate firing modes for Vampire and Violator.
* New vehicle: Abaddon Hovertank.
* Plasma Beam. (Abaddon's main cannon.)
* Flamethrower. (Abaddon's gunner weapon.)
* Railgun fires through multiple targets like in other Quake games.
* Alternate firing mode for Strogg Tactical Shield. Absorbs shields.
* Crosshairs optionally removed.
* Hitbeep notifications optionally removed.
* Vehicles use depletable ammunition.
* Bot AI improvements.
* BaseETQW bugfixes.

3. Installation
================================
First off, you'll want to unzip the QWTA release zip file into your 
ETQW folder. You should end up with a /qwta/ folder alongside your /base/ 
folder, inside your ETQW folder. 

Next, simply start ETQW as normal, and use the Mod Menu to select QWTA.
Alternatively, you can start ETQW with the commandline: +set fs_game qwta

Finally, find yourself a server that's running QWTA, and give things a try. :)
You can also simply start up ETQW, find a server running QWTA, and connect.
The server will auto-download the mod and install it for you.

4. Known Issues
================================
Lots, at present. QWTA v0.3.3 is a bit of a rush-job release as some of
the more severe issues with QWTA v0.3.2 were discovered during the development
of QWTA v0.4.0. So v0.4.0 was put on hold and v0.3.3 was released.
To be honest, I probably should have released just a tiny patch, but some of
the new v0.4.0 features seemed enjoyable to include.
Without further ado, the complete list:
* Magog-carried GDF vehicle will fail to deploy if the vehicle is destroyed.
* Bots don't wait for their vehicle drop to arrive before moving on.
* 3rd Eye Camera updates its radar origin even when not moving.
* Players killed by being smashed against walls don't attribute frags properly
* Dark Matter Cannon's gravity won't pull players away from mounted guns.
* Dark Matter Cannon will (very slightly) damage nearby objects when fired.
* Bots vehicle charge bars don't reset when going from warmup to ingame.
* FE costs are not listed anywhere ingame.
* Hyperblaster projectiles continue to home in on a freshly respawned player.
* Blood effects' player wound decals are not copied to corpses.
* Blood effects' splatter effects are not properly scaled or rate limited.
* Blood effects' decals don't get cleared until after a corpse is decomposed.
* Blood effects' player wound decals are not applied for knife wounds.
* Blood effects' player wound decals will spam console with warning messages.
* Hog, Desecrator, and Abaddon teleport-drop use a simplified effect.
* Abaddon Flamethrower uses very simplified physics for now.
* There's no cap on the number of vehicles dropped by different players.
* Documentation tab on Limbo menu QWTA page, is blank.

5. Credits
================================
Azuvector
-  Hi, this is me. Programmer, scripter, mod-founder, readme-writer, whatever.

Special thanks to:
jRAD
digibob
RR2
- Some of the Splash Damage folks who hang around on IRC and the community 
- forums where they've been gracious enough to put up with newbie questions 
- from me.
hannes
jaybird
Mxyzptlk
Orange
reyalP
TimeStar
WhiteAden
- A few other ETQW modders who've provided helpful advice and assistance.
Id Software
Splash Damage
Raven Software