# `CombatAssist`
[![Platform](https://img.shields.io/static/v1?label=platform&message=windows&color=dimgray&style=flat)](#)

A [KCSE](https://github.com/JerryYOJ/KCSE) plugin for **Kingdom Come: Deliverance 1** that automates perfect blocks and master strikes via configurable console variables.

## Dependencies

- [KCSE](https://github.com/JerryYOJ/KCSE)
- [libKCD1](https://github.com/JerryYOJ/libKCD1)

## Installation

1. Install KCSE (`dinput8.dll` in `<game>/Bin/Win64/`)
2. Copy `CombatAssist.dll` to `<game>/mods/<modname>/KCSE/Plugins/`

## Console Variables

All CVars are set via the in-game console (`~`).

| CVar | Default | Description |
|------|---------|-------------|
| `kcse_ca_alwaysPB` | 0 | Always auto perfect block |
| `kcse_ca_alwaysMS` | 0 | Always auto master strike |
| `kcse_ca_chancePB` | 0 | 0–100: % chance to auto perfect block |
| `kcse_ca_chanceMS` | 0 | 0–100: % chance to auto master strike |
| `kcse_ca_alwaysHuntPB` | 0 | Auto PB during hunt/gap-close attacks |
| `kcse_ca_alwaysHuntMS` | 0 | Auto MS during hunt attacks |
| `kcse_ca_autoCounter` | 0 | Auto-react N times then stop (decrements each use) |

Master strikes are preferred over perfect blocks when both are available.

## License

[GPLv3](LICENSE)
