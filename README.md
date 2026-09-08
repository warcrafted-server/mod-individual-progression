<p align="center">
  <a href="https://github.com/warcrafted-server">
    <img src="https://raw.githubusercontent.com/warcrafted-server/WarCrafted-ControlP/main/Logo_github.jpg" alt="WarCrafted Universe Header" />
  </a>
</p>

<p align="center">
  🌐 <b>Idiomas / Languages:</b> <a href="README.es.md">Español 🇪🇸</a> | <a href="README.md">English 🇬🇧</a>
</p>

---

- Estado de la última compilación con azerothcore: [![Build Status](https://github.com/warcrafted-server/mod-individual-progression/actions/workflows/core-build.yml/badge.svg?branch=master)](https://github.com/warcrafted-server/mod-individual-progression/actions) ![WoW Version](https://img.shields.io/badge/WoW-3.3.5a-blue) ![Last Commit](https://img.shields.io/github/last-commit/warcrafted-server/mod-individual-progression)

# mod-individual-progression

An AzerothCore module that simulates progress through expansions and expansion tiers on a per-character basis. Each character advances through Vanilla, TBC, and WotLK content independently — killing the appropriate tier-ending boss unlocks the next stage — instead of the server exposing all 1-80 content on day one. Built for private servers that want a classic-style leveling journey without running separate progression realms, and pairs well with Playerbots and NPCBots.

---

## Features

- **Per-character progression** — Each character tracks its own progression tier (0-18) via Player Settings, independent of the realm's expansion. Killing a tier-ending boss (Ragnaros, Onyxia, Nefarian, C'Thun, Kel'Thuzad, Kael'thas, Illidan, Kil'jaeden, Yogg-Saron, the Lich King...) advances the character to the next tier.
- **Level caps per era** — XP gain stops at 60 until Vanilla is cleared (`PROGRESSION_PRE_TBC`) and at 70 until TBC is cleared (`PROGRESSION_TBC_TIER_5`). Outland and Northrend are inaccessible before their respective tiers.
- **No catch-up mechanics** — Quest XP inflation from patches 2.3.0 and 3.0.2 is reverted (`QuestXPFix`), gear stats are restored to their original era values, and shortcuts to end-game content are closed. The point is the journey through each expansion, not rushing to the level cap.
- **Restored content** — Original Naxxramas (40-man), Onyxia, and TBC attunement quest chains; outdoor elite creatures; classic riding skill requirements; Vanilla Alterac Valley; Vanilla PvP honor ranks and rank-based titles/rewards.
- **Optional power/healing adjustments** — Separate multipliers for Vanilla and TBC attack power and healing, to approximate original-era difficulty (`VanillaPowerAdjustment`, `TBCPowerAdjustment`, etc.). Can be scoped to bots only (`BotOnlyAdjustments`) to raise dungeon/raid difficulty without nerfing real players.
- **WotLK emblem gating** — Emblem drops (Heroism/Valor/Conquest/Triumph/Frost) follow the player's progression tier rather than the realm's patch level.
- **Molten Core manual rune handling** — Optional (`ManualRuneHandling`) requirement to douse the seven Molten Core runes with Aqual Quintessence before Ragnaros is summonable, with a configurable cooldown reduction.
- **Naxxramas 40 difficulty toggles** — Individual flags to ease the Four Horsemen, Gluth, Patchwerk, and Razuvious encounters for groups that can't run the original 40-man strategies (see [Naxxramas 40 boss toggles](#naxxramas-40-boss-toggles) below).
- **Isle of Quel'Danas progression** — Dailies and factions unlock in phases matching the original patch 2.4 rollout.
- **Playerbots support** — Bot accounts (matched by regex) are excluded from progression gating entirely, so they can serve any tier at their configured max level.
- **Progression-aware RDF (Dungeon Finder)** — See [Random Dungeon Finder gating](#random-dungeon-finder-gating) below.
- **Alterac Valley turn-in economy** — The Vanilla supply-line battleground, restored. See [Alterac Valley turn-ins](#alterac-valley-turn-ins) below.
- **Restored encounters** — Attumen and Midnight in Karazhan (with the mount-up transition), Magmus in Blackrock Depths, and the "An Earnest Proposition" Spectral Stalker ambush on the Dungeon Set 2 turn-in.
- **Custom progression chains** — Optionally replace the default boss-kill progression with a list of custom creature-to-tier mappings (`CustomProgression`).

Full changelog and tier reference: [List of Changes](https://github.com/ZhengPeiRu21/mod-individual-progression/wiki/List-of-Changes) and [List of Progression Tiers](https://github.com/ZhengPeiRu21/mod-individual-progression/wiki/List-of-Progression-Tiers) (upstream wiki).

---

## Progression Tiers

| Tier | Unlocks after |
|------|---------------|
| 0 | Start (Vanilla, pre-raid) |
| 1 | Ragnaros → Blackwing Lair |
| 2 | Onyxia |
| 3 | Nefarian → Zul'Gurub, AQ War Effort |
| 4-6 | AQ gates, AQ War, C'Thun → Naxxramas 40, Scourge Invasion |
| 7 | Kel'Thuzad → "Into the Breach" quest |
| **8** | **Dark Portal opens** → Karazhan, Gruul's Lair, Magtheridon's Lair |
| 9-10 | Serpentshrine Cavern/Tempest Keep → Hyjal Summit, Black Temple |
| 12 | Sunwell Plateau |
| **13** | **WotLK unlocked** → Naxxramas (10/25), Eye of Eternity, Obsidian Sanctum |
| 14-18 | Ulduar → Trial of the Crusader → Icecrown Citadel → Ruby Sanctum |

Tier 11 (Zul'Aman) is intentionally unused in the base enum — see `RequiredZulAmanProgression` below for how ZA is gated instead.

---

## Requirements

- AzerothCore (WotLK 3.3.5a)
- `EnablePlayerSettings = 1` in `worldserver.conf` — **required**. Progression is stored per-character via Player Settings; without this, progress silently fails to save.
- `DBC.EnforceItemAttributes = 0` in `worldserver.conf` — **required** for restored Vanilla/TBC item stats to take effect. Both of these are set automatically at startup if `IndividualProgression.SimpleConfigOverride = 1` (default), but setting them directly in `worldserver.conf` avoids depending on that override.

---

## Installation

1. Clone or copy this module into your `modules/` directory:
   ```
   modules/mod-individual-progression/
   ```

2. Rebuild the server:
   ```bash
   cd build
   cmake ..
   make -j$(nproc)
   make install
   ```

3. Apply the SQL data files under `data/sql/` to the appropriate databases (world/characters), following the module's `data/sql` directory structure.

4. Copy and edit the config:
   ```
   env/dist/etc/modules/individualProgression.conf
   ```

5. Restart the worldserver.

### Optional Data Files

Some Vanilla/TBC restorations require DBC modification — see the `optional/` directory:

- `patch-V.7z` / `patch-S.7z` — client-side MPQ patches restoring pre-3.1 reagent requirements (e.g. Flint and Steel for campfires), Vanilla enchanting reagents, and profession leveling curves.
- `dbc.7z` — corresponding server-side DBC changes.
- `patch-explanations.txt` — details what each optional patch changes.

All of these are optional; the module functions without them, but the affected recipes/requirements stay at their WotLK values. Client-side MPQ patches must be installed on the player's client — this is not something a server-side deploy can apply.

**Optional recommended addon:** [hide-vendor-price](https://github.com/ZhengPeiRu21/hide-vendor-price) restores pre-3.2 behavior where item prices only display in tooltips at a vendor.

---

## Configuration

The full annotated config lives in `conf/individualProgression.conf.dist`. Key options:

| Option | Default | Description |
|--------|---------|--------------|
| `IndividualProgression.Enable` | `1` | Enable the module. Database changes made by the module (restored quests, NPCs, etc.) persist even if later disabled. |
| `IndividualProgression.EnforceGroupRules` | `0` | Only allow grouping with players in the same progression phase. |
| `IndividualProgression.VanillaPowerAdjustment` / `VanillaHealingAdjustment` | `1` | Multiplier on attack power / healing before the Dark Portal opens (levels 11-60). `1` = no adjustment; blizzlike Vanilla feel is closer to `0.5-0.6`. |
| `IndividualProgression.TBCPowerAdjustment` / `TBCHealingAdjustment` | `1` | Same, for TBC content after the Dark Portal but before TBC is cleared. |
| `IndividualProgression.BotOnlyAdjustments` | `0` | Scope the above power/healing adjustments to RNDbots only, leaving real players unaffected. |
| `IndividualProgression.QuestXPFix` | `1` | Revert the quest XP boosts added in patches 2.3.0/3.0.2. |
| `IndividualProgression.RequireNaxxStrathEntrance` | `0` | Require entering Naxxramas 40 via the original Stratholme back entrance, consistent with MC/BWL. |
| `IndividualProgression.doableNaxx40Bosses_4H` / `_Gluth` / `_Patchwerk` / `_Razuvious` | `0` | See [Naxxramas 40 boss toggles](#naxxramas-40-boss-toggles). |
| `IndividualProgression.MoltenCore.ManualRuneHandling` | `1` | Require manually dousing MC runes with Aqual Quintessence rather than automatic unlock on boss kill. |
| `IndividualProgression.MoltenCore.AqualEssenceCooldownReduction` | `0` | Minutes to shave off the 1-hour Aqual Quintessence cooldown. `60` removes the cooldown entirely. |
| `IndividualProgression.SerpentshrineCavern.RequireAllBosses` / `TheEye.RequireAllBosses` | `1` | Require all prior bosses dead before Vashj's console / Kael's door unlocks. |
| `IndividualProgression.FishingFix` | `1` | Restore pre-3.1 fishing skill zone-progression requirements. |
| `IndividualProgression.VanillaHunterPets` | `0` | Require taming specific beasts to learn pet spells/ranks, as in Vanilla/TBC. **Disabled and unusable in this fork** — see [Features not carried over](#features-not-carried-over). |
| `IndividualProgression.WarlockDemonTrainers` | `0` | Require buying grimoires for demon spells, as in Vanilla/TBC. **Disabled and unusable in this fork** — see [Features not carried over](#features-not-carried-over). |
| `IndividualProgression.AV.ScrapsSeasoned` / `ScrapsVeteran` / `ScrapsChampion` | `25` / `50` / `75` | Armor Scrap turn-ins (20 scraps each) needed before graveyard defenders upgrade to the next tier. Upstream suggests `12` / `24` / `36` for smaller populations. |
| `IndividualProgression.AV.BossPointsRequired` | `200` | Turn-in points needed to summon Ivus the Forest Lord / Lokholar the Ice Lord. A 5-item turn-in is 5 points, a single turn-in is 1. Upstream suggests `100`. |
| `IndividualProgression.AV.AirStrikeTurnIns` | `50` | Call of Air turn-ins per fleet, per match, before that Wing Commander hands out his air strike beacon. The receiving player must be Honored with Frostwolf Clan / Stormpike Guard. |
| `IndividualProgression.AV.StablesTurnIns` | `10` | Empty Stables and Ram Harness turn-ins required to send the cavalry out. |
| `IndividualProgression.AV.GossipExactNumbers` | `0` | The supplies gossip on Smith Regzar / Murgot Deepforge answers with an estimate. Set to `1` to show the exact turn-in count, useful while tuning the thresholds. |
| `IndividualProgression.SimpleConfigOverride` | `1` | Automatically set `EnablePlayerSettings`, `DBC.EnforceItemAttributes`, and the water breath timer at startup. Disable only if you're setting these yourself in `worldserver.conf`. |
| `IndividualProgression.DisableQuestMarkers` | `1` | Disable quest object markers/sparkles (added in patch 2.3), for a more Vanilla quest-hunting experience. |
| `IndividualProgression.DisableRDF` | `0` | See [Random Dungeon Finder gating](#random-dungeon-finder-gating). |
| `IndividualProgression.MaxMonsterSight` | `1` | Raise monster sight range from 50 to 80 yards (needed for Alterac Valley tower/bunker archers). Does not affect aggro range. |
| `IndividualProgression.QuestMoneyAtLevelCap` | `1` | Grant extra quest gold at the current progression tier's level cap, as in patch 1.10. |
| `IndividualProgression.RepeatableVanillaQuestsXP` | `1` | Allow repeatable Vanilla quests to grant XP on every turn-in (Vanilla behavior) instead of once (WotLK behavior). |
| `IndividualProgression.ProgressionLimit` | `0` | Cap the maximum progression tier a character can reach (1-17). `0` = no cap (tier 18). |
| `IndividualProgression.StartingProgression` | `0` | Set the tier new/existing characters start at. |
| `IndividualProgression.TbcRacesUnlockProgression` / `tbcRacesStartingProgression` | `0` / `0` | Tier at which Draenei/Blood Elves become creatable, and the tier they start at. |
| `IndividualProgression.DeathKnightUnlockProgression` / `DeathKnightStartingProgression` | `13` / `13` | Tier at which Death Knights become creatable and the tier they start at. Defaults to requiring TBC completion, since DKs aren't balanced for earlier content. |
| `IndividualProgression.RequiredZulGurubProgression` | `3` | Tier required to enter Zul'Gurub (default: after BWL). |
| `IndividualProgression.RequiredZulAmanProgression` | `12` | Tier required to enter Zul'Aman (default: after Black Temple). Suggested `9` to open ZA right after Karazhan instead, matching its intended gear level. |
| `IndividualProgression.DisableDefaultProgression` | `0` | Disable the standard boss-kill progression flow in favor of `CustomProgression` entries only. |
| `IndividualProgression.CustomProgression` | `""` | Space/comma-separated `creatureId tier` pairs for custom progression triggers, e.g. `"639 8, 3975 13"`. |
| `IndividualProgression.AllowEarlyDungeonSet2` | `0` | Make the Dungeon Set 2 quest chain available before Blackwing Lair is cleared, so the gear isn't already obsolete when unlocked. |
| `IndividualProgression.AllowEarlyScourgeBosses` | `0` | Make the 6 Scourge Invasion dungeon bosses available outside their narrow tier-6 window. |
| `IndividualProgression.TBC.ArenaSeason` / `WotLK.ArenaSeason` | `1` / `5` | Active arena season IDs per expansion. |
| `IndividualProgression.VanillaPvpKillRequirement.Rank1`-`Rank14` | see `.conf.dist` | Honor kill thresholds for each Vanilla PvP rank/title. |
| `IndividualProgression.VanillaPvpTitlesPersistAfterVanilla` | `1` | Keep earned Vanilla PvP titles after reaching TBC/WotLK. |
| `IndividualProgression.VanillaPvpEarnTitlesAfterVanilla` | `0` | Allow earning new Vanilla PvP titles after leaving Vanilla. |
| `IndividualProgression.BotAccountsEarnPvPTitles` | `0` | Allow bot accounts to earn PvP titles. |
| `IndividualProgression.BotAccountsMaxLevel` | `80` | Max level for bot accounts. |
| `IndividualProgression.BotAccountsRegex` | `"^RNDBOT.*"` | Regex matching bot account names, excluded from progression gating entirely. |
| `IndividualProgression.ExcludedAccountsRegex` | `""` | Regex matching additional accounts (e.g. test accounts) to exclude from progression gating. |
| `IndividualProgression.EnableAllSpellRanks` | `0` | Allow casting Vanilla/TBC spell ranks during WotLK. Only relevant if the optional `patch-V` static-mana-cost restoration is installed. |
| `IndividualProgression.EnableSetRepCommand` | `0` | Enable `.ip setrep`, letting a player copy an account character's reputation to another. |
| `IndividualProgression.LimitedSetRepCommand` | `1` | Require the target character to already be neutral/friendly/honored with a faction (depending on faction) before `.ip setrep` can raise it further — prevents multiplying reputation via repeated quest turn-ins. |
| `IndividualProgression.sharedFactionIdsRegex` | see `.conf.dist` | Faction IDs eligible for `.ip setrep` sharing. |

---

## Alterac Valley Turn-Ins

Vanilla Alterac Valley was a war of attrition with a supply line, not a 20-minute race to the enemy general. Patch 2.3 and the 3.x revisions stripped most of that out. This restores it, and all of it is driven from the world database and module scripts — no core changes, no client patch.

**Armor Scraps → better defenders.** Corpses drop Armor Scraps (Enemy Booty for the Horde). Turn them in 20 at a time to Smith Regzar (Horde) or Murgot Deepforge (Alliance). Once your side has banked enough, every graveyard and tower defender upgrades a tier: Seasoned → Veteran → Champion, each with more health and damage.

**Crystals and blood → the elemental boss.** The Alliance turns in Crystal Clusters to summon Ivus the Forest Lord; the Horde turns in Stormpike Soldier's Blood for Lokholar the Ice Lord. Each faction has a 5-item turn-in worth 5 points and a single turn-in worth 1.

**Call of Air → air strikes.** Six Wing Commanders, three per side (Guse, Jeztor, Mulverick for the Horde; Ichman, Slidore, Vipore for the Alliance), each with their own turn-in quest counted per fleet. When a fleet banks enough, its commander hands out a beacon targeting a fixed location: Snowfall Graveyard, the east crater, or the west crater. Receiving one requires Honored with Frostwolf Clan or Stormpike Guard.

**Stables and harnesses → cavalry.** Turn Frostwolf Muzzles or Stormpike Collars in to the stable master and the Cavalry Commander rides out with a charge of Frostwolf wolves or Alterac rams.

**The mines and the caves.** Irondeep and Coldtooth are rebuilt in full — troggs, shamans, foremen, guards, miners, their bosses (Morloch, Keetar, Aggi Rumblestomp, Masha Swiftcut), patrols and formations — along with the Wildpaw gnolls, Snowblind harpies and the Alterac Yeti. Holding a mine feeds resources to your side, which is the other half of the AV economy.

Smith Regzar and Murgot Deepforge also gain a gossip option reporting how the supply effort is going. By default the answer is an estimate; `AV.GossipExactNumbers = 1` gives the precise count.

Note that every default here is calibrated for 40-vs-40 with real players. On a small or bot-driven realm, lower the thresholds from day one or a match will end before anything upgrades.

---

## Features Not Carried Over

Two upstream features are deliberately absent from this fork because they cannot work without shipping a modified client:

- **Vanilla hunter pet training** (`hunter_pet_trainers.sql`) registers roughly 76 pet spells that do not exist in the 3.3.5 client — Claw and Bite ranks, Charge, Dash, Dive ranks, and others upstream's own comments mark as *"don't exist in wotlk"*.
- **Warlock grimoires** (`warlock_pet_trainers.sql`) does the same with roughly 83 demon spells.

Both need the optional `patch-V` client MPQ installed on every player's machine to render those spells. The SQL files are not shipped here and `VanillaHunterPets` / `WarlockDemonTrainers` both default to `0`. The supporting code is still present and inert: both entry points return immediately when their option is off, and the pet spell checks only ever act on the 600000-700000 ID range, which no character can hold without those SQL files. If you do distribute `patch-V`, take the two SQL files from upstream and flip the options to `1`.

---

## Naxxramas 40 Boss Toggles

The original 40-man Naxxramas encounters relied on precise, human-coordinated strategies. These optional flags ease four fights for groups (particularly bot-heavy raids) that can't execute the original mechanics — but they don't all have the same effect:

| Flag | What it changes | Does it make the fight doable? |
|------|------------------|----------------------------------|
| `doableNaxx40Bosses_4H` | Mark of Zeliek damage per stack drops ~10x (e.g. 1000→100 at 3 stacks); Zeliek's Holy Wrath drops from 443 to 222; Blaumeux's Void Zone is no longer cast. | **Yes.** The Mark of the Horsemen mechanic is what makes this fight a positioning puzzle (four tanks rotating corners); cutting its damage an order of magnitude turns it into a straightforward tank-and-spank. Removing the Void Zone also matters for bots, which don't reliably step out of ground AoE until already taking damage from it. |
| `doableNaxx40Bosses_Gluth` | Gluth's zombie-eating check (which heals him 5% max HP per zombie consumed) runs every 12s instead of every 3s. | **Yes.** At the original 3s interval, incoming zombie heals typically outpace raid DPS. Quadrupling the interval makes the DPS race winnable. |
| `doableNaxx40Bosses_Patchwerk` | Hateful Strike damage drops ~20% (from 22100-22850 to 17680-18280). | **Not by itself.** Hateful Strike always targets whichever melee-range unit has the highest HP while skipping the top-threat target — a 20% cut helps but doesn't change that mechanic. The real fix for bot raids is keeping only the tank in melee range: with a single melee target, every Hateful Strike lands on the tank, turning it into an ordinary tank/healer check. |
| `doableNaxx40Bosses_Razuvious` | Razuvious casts Disrupting Shout (damage) instead of Mana Burn every 25s. | **Mostly.** Mana Burn drains healer mana faster than most bot healers can recover, typically ending the fight via mana exhaustion rather than a wipe. Disrupting Shout is ordinary damage that any healer can handle. This flag does *not* remove the Understudies' mind-control requirement, which is unrelated to bot healer mana management. |

---

## Random Dungeon Finder Gating

`IndividualProgression.DisableRDF = 1` blocks the in-client Dungeon Finder (`DungeonFinder.OptionsMask` is forced to `4` — seasonal bosses only — at startup) so players can't queue into content outside their progression tier or collect the RDF daily reward/emblems early.

**This gate is progression-aware, not a permanent switch.** A `GlobalScript::OnInitializeLockedDungeons` hook re-locks every non-seasonal dungeon/raid for a character until it reaches `PROGRESSION_TBC_TIER_5` (tier 13 — WotLK Naxxramas/Eye of Eternity/Obsidian Sanctum unlocked). This mirrors the finder's real launch timing: the automated, cross-realm Dungeon Finder shipped with patch 3.3.0 (Icecrown Citadel), well after early WotLK raids — but gating it to ICC's own tier would leave a large chunk of WotLK content (Naxx 10/25, Ulduar, ToC) without any queueing tool at all. Tier 13 is the earliest point that's still unambiguously "in WotLK" while giving players a finder for the rest of the expansion.

Bot accounts (matched by `BotAccountsRegex`) and GMs are exempt and always see dungeons as unlocked, consistent with how progression gating works everywhere else in this module.

Note that `.reload config` does **not** re-apply `DungeonFinder.OptionsMask` to the running LFG system — the option mask is only read once, at `LFGMgr` construction. If you need to change the realm-wide mask live, use `.lfg options <mask>` instead; the per-character tier gate above works independently of that mask and re-evaluates on every dungeon-lock check.

---

## Commands

All commands are under `.ip` and require GM security level (`SEC_GAMEMASTER`).

| Command | Description |
|---------|-------------|
| `.ip get [player]` | Show a player's current progression tier. |
| `.ip set [player] <tier>` | Set a player's progression tier directly. |
| `.ip setbot` | Progression-related bot account management. |
| `.ip setrep` | Copy reputation from the account's highest-reputation character (see `EnableSetRepCommand`/`LimitedSetRepCommand`). |
| `.ip pvp [player]` | PvP rank/title related actions. |
| `.ip tele [player] <location>` | Teleport to a progression-related location. |
| `.ip attune <location>` | Attunement-related actions. |

---

## Discord

[https://discord.gg/PTXxh3KSgQ](https://discord.gg/PTXxh3KSgQ) (upstream project Discord)

---

## Credits

* AzerothCore and TrinityCore, along with ancestors Mangos.
* AzerothCore mod-war-effort
* AzerothCore mod-pvp-titles
* vMangos for Vanilla game and patch progression data
* cMangos for TBC game data
* Sogladev for Naxx 40 scripts and data
* Original module: [ZhengPeiRu21/mod-individual-progression](https://github.com/ZhengPeiRu21/mod-individual-progression)
* [Grimfeather/mod-individual-progression](https://github.com/Grimfeather/mod-individual-progression) for the Alterac Valley turn-in system, the Attumen/Midnight and Magmus encounters, and many world fixes carried over here

---

## License

MIT
