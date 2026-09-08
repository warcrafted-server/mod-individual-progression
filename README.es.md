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

Un módulo de AzerothCore que simula el avance por las expansiones y sus tiers de contenido de forma individual, personaje a personaje. Cada personaje progresa por Vanilla, TBC y WotLK de manera independiente — matar al jefe final de cada tier desbloquea la siguiente etapa — en lugar de que el servidor exponga todo el contenido de nivel 1-80 desde el primer día. Pensado para servidores privados que quieren una experiencia de progresión al estilo clásico sin mantener reinos de progresión separados, y funciona bien junto a Playerbots y NPCBots.

---

## Características

- **Progresión por personaje** — Cada personaje guarda su propio nivel de progresión (0-18) mediante Player Settings, independientemente de la expansión del reino. Matar al jefe final de un tier (Ragnaros, Onyxia, Nefarian, C'Thun, Kel'Thuzad, Kael'thas, Illidan, Kil'jaeden, Yogg-Saron, el Rey Exánime...) hace avanzar al personaje al siguiente tier.
- **Límites de nivel por era** — La ganancia de experiencia se detiene en el nivel 60 hasta completar Vanilla (`PROGRESSION_PRE_TBC`) y en el 70 hasta completar TBC (`PROGRESSION_TBC_TIER_5`). Terrallende y Rasganorte son inaccesibles antes de alcanzar su tier correspondiente.
- **Sin mecánicas de ponerse al día (catch-up)** — Se revierte el aumento de XP de misiones introducido en los parches 2.3.0 y 3.0.2 (`QuestXPFix`), las estadísticas del equipo se restauran a los valores originales de su era, y se cierran los atajos al contenido de máximo nivel. La idea es vivir el viaje por cada expansión, no llegar corriendo al nivel máximo.
- **Contenido restaurado** — Naxxramas original (40 jugadores), Onyxia y las cadenas de misiones de sintonización de TBC; criaturas de élite en zonas exteriores; requisitos clásicos de la habilidad de montar; Vale de Alterac de Vanilla; rangos de honor PvP de Vanilla con sus títulos y recompensas asociadas.
- **Ajustes opcionales de poder/curación** — Multiplicadores independientes para el poder de ataque y la curación durante Vanilla y TBC, para aproximar la dificultad original de cada era (`VanillaPowerAdjustment`, `TBCPowerAdjustment`, etc.). Se pueden limitar solo a los bots (`BotOnlyAdjustments`) para aumentar la dificultad de mazmorras/bandas sin penalizar a los jugadores reales.
- **Emblemas de WotLK condicionados** — La caída de emblemas (Heroísmo/Valor/Conquista/Triunfo/Escarcha) sigue el nivel de progresión del jugador en lugar del parche del reino.
- **Manejo manual de las runas de Molten Core** — Requisito opcional (`ManualRuneHandling`) de apagar las siete runas de Molten Core con la Quintaesencia Ácuea antes de poder invocar a Ragnaros, con una reducción de tiempo de reutilización configurable.
- **Interruptores de dificultad en Naxxramas 40** — Opciones individuales para suavizar los combates de los Cuatro Jinetes, Gluth, Patchwerk y Razuvious, pensadas para grupos que no pueden ejecutar las estrategias originales de 40 jugadores (ver [Interruptores de jefes de Naxxramas 40](#interruptores-de-jefes-de-naxxramas-40) más abajo).
- **Progresión de la Isla de Quel'Danas** — Las misiones diarias y facciones se desbloquean por fases, igual que en el lanzamiento original del parche 2.4.
- **Compatibilidad con Playerbots** — Las cuentas de bots (identificadas por expresión regular) quedan completamente excluidas del control de progresión, de modo que pueden servir en cualquier tier con su nivel máximo configurado.
- **Buscador de mazmorras (RDF) consciente de la progresión** — Ver [Restricción del Buscador de Mazmorras Aleatorio](#restricción-del-buscador-de-mazmorras-aleatorio-rdf) más abajo.
- **Cadenas de progresión personalizadas** — Permite sustituir la progresión estándar basada en muertes de jefes por una lista de correspondencias criatura → tier definida por el operador (`CustomProgression`).

Lista de cambios completa y referencia de tiers: [List of Changes](https://github.com/ZhengPeiRu21/mod-individual-progression/wiki/List-of-Changes) y [List of Progression Tiers](https://github.com/ZhengPeiRu21/mod-individual-progression/wiki/List-of-Progression-Tiers) (wiki original, en inglés).

---

## Niveles de progresión

| Tier | Se desbloquea tras |
|------|---------------|
| 0 | Inicio (Vanilla, sin bandas) |
| 1 | Ragnaros → Ala Negra |
| 2 | Onyxia |
| 3 | Nefarian → Zul'Gurub, Esfuerzo de Guerra de AQ |
| 4-6 | Puertas de AQ, Guerra de AQ, C'Thun → Naxxramas 40, Invasión de la Plaga |
| 7 | Kel'Thuzad → misión "Hacia la Brecha" |
| **8** | **Se abre el Portal Oscuro** → Karazhan, Cubil de Gruul, Cubil de Magtheridon |
| 9-10 | Caverna de Aguasrresplandecientes/Fuerte Tempestuoso → Cumbre de Hyjal, Templo Negro |
| 12 | Meseta de la Fuente del Sol |
| **13** | **Se desbloquea WotLK** → Naxxramas (10/25), Ojo de la Eternidad, Santuario de Obsidiana |
| 14-18 | Ulduar → Prueba del Cruzado → Ciudadela de la Corona de Hielo → Santuario Rubí |

El tier 11 (Zul'Aman) queda deliberadamente sin usar en la enumeración base — ver `RequiredZulAmanProgression` más abajo para cómo se controla el acceso a ZA en su lugar.

---

## Requisitos

- AzerothCore (WotLK 3.3.5a)
- `EnablePlayerSettings = 1` en `worldserver.conf` — **obligatorio**. La progresión se guarda por personaje mediante Player Settings; sin esto, el progreso no se guarda y no da ningún error visible.
- `DBC.EnforceItemAttributes = 0` en `worldserver.conf` — **obligatorio** para que las estadísticas restauradas de objetos de Vanilla/TBC surtan efecto. Ambos valores se aplican automáticamente al arrancar si `IndividualProgression.SimpleConfigOverride = 1` (por defecto), pero configurarlos directamente en `worldserver.conf` evita depender de ese override.

---

## Instalación

1. Clona o copia este módulo dentro de tu directorio `modules/`:
   ```
   modules/mod-individual-progression/
   ```

2. Recompila el servidor:
   ```bash
   cd build
   cmake ..
   make -j$(nproc)
   make install
   ```

3. Aplica los archivos SQL en `data/sql/` a las bases de datos correspondientes (world/characters), siguiendo la estructura de directorios `data/sql` del módulo.

4. Copia y edita la configuración:
   ```
   azerothcore/modules/individualProgression.conf
   ```

5. Reinicia el worldserver.

### Archivos de datos opcionales

Algunas restauraciones de Vanilla/TBC requieren modificar DBCs — ver el directorio `optional/`:

- `patch-V.7z` / `patch-S.7z` — parches MPQ del lado del cliente que restauran requisitos de reactivos anteriores al 3.1 (por ejemplo, Pedernal y Yesca para hogueras), reactivos de encantamiento de Vanilla, y las curvas de subida de nivel de profesiones.
- `dbc.7z` — cambios de DBC correspondientes en el lado del servidor.
- `patch-explanations.txt` — detalla qué cambia cada parche opcional.

Todo esto es opcional; el módulo funciona sin ello, pero las recetas/requisitos afectados se quedan con sus valores de WotLK. Los parches MPQ del cliente deben instalarse en el cliente del jugador — esto no es algo que un despliegue del lado del servidor pueda aplicar.

**Addon recomendado opcional:** [hide-vendor-price](https://github.com/ZhengPeiRu21/hide-vendor-price) restaura el comportamiento anterior al 3.2, en el que los precios de los objetos solo se muestran en los tooltips estando junto a un vendedor.

---

## Configuración

La configuración completa y comentada está en `conf/individualProgression.conf.dist`. Opciones principales:

| Opción | Por defecto | Descripción |
|--------|---------|--------------|
| `IndividualProgression.Enable` | `1` | Activa el módulo. Los cambios en la base de datos hechos por el módulo (misiones restauradas, NPCs, etc.) persisten aunque se desactive después. |
| `IndividualProgression.EnforceGroupRules` | `0` | Solo permite agruparse con jugadores en la misma fase de progresión. |
| `IndividualProgression.VanillaPowerAdjustment` / `VanillaHealingAdjustment` | `1` | Multiplicador de poder de ataque / curación antes de abrir el Portal Oscuro (niveles 11-60). `1` = sin ajuste; la sensación blizzlike de Vanilla se acerca más a `0.5-0.6`. |
| `IndividualProgression.TBCPowerAdjustment` / `TBCHealingAdjustment` | `1` | Lo mismo, para contenido de TBC tras abrir el Portal Oscuro pero antes de completar TBC. |
| `IndividualProgression.BotOnlyAdjustments` | `0` | Limita los ajustes de poder/curación anteriores solo a los RNDbots, sin afectar a los jugadores reales. |
| `IndividualProgression.QuestXPFix` | `1` | Revierte los aumentos de XP de misiones añadidos en los parches 2.3.0/3.0.2. |
| `IndividualProgression.RequireNaxxStrathEntrance` | `0` | Exige entrar a Naxxramas 40 por la entrada original trasera de Stratholme, de forma consistente con MC/BWL. |
| `IndividualProgression.doableNaxx40Bosses_4H` / `_Gluth` / `_Patchwerk` / `_Razuvious` | `0` | Ver [Interruptores de jefes de Naxxramas 40](#interruptores-de-jefes-de-naxxramas-40). |
| `IndividualProgression.MoltenCore.ManualRuneHandling` | `1` | Exige apagar manualmente las runas de MC con la Quintaesencia Ácuea en lugar de desbloqueo automático al matar al jefe. |
| `IndividualProgression.MoltenCore.AqualEssenceCooldownReduction` | `0` | Minutos que se restan del tiempo de reutilización de 1 hora de la Quintaesencia Ácuea. `60` elimina el tiempo de reutilización por completo. |
| `IndividualProgression.SerpentshrineCavern.RequireAllBosses` / `TheEye.RequireAllBosses` | `1` | Exige que todos los jefes anteriores estén muertos antes de que se desbloquee la consola de Vashj / la puerta de Kael. |
| `IndividualProgression.FishingFix` | `1` | Restaura los requisitos de progresión por zona de la habilidad de pesca anteriores al 3.1. |
| `IndividualProgression.SimpleConfigOverride` | `1` | Establece automáticamente `EnablePlayerSettings`, `DBC.EnforceItemAttributes` y el tiempo de aguante bajo el agua al arrancar. Desactívalo solo si vas a configurar estos valores tú mismo en `worldserver.conf`. |
| `IndividualProgression.DisableQuestMarkers` | `1` | Desactiva los marcadores y destellos de objetos de misión (añadidos en el parche 2.3), para una experiencia de búsqueda de misiones más fiel a Vanilla. |
| `IndividualProgression.DisableRDF` | `0` | Ver [Restricción del Buscador de Mazmorras Aleatorio](#restricción-del-buscador-de-mazmorras-aleatorio-rdf). |
| `IndividualProgression.MaxMonsterSight` | `1` | Aumenta el rango de visión de los monstruos de 50 a 80 yardas (necesario para los arqueros de torres/búnkeres del Vale de Alterac). No afecta al rango de agresión. |
| `IndividualProgression.QuestMoneyAtLevelCap` | `1` | Concede oro adicional de misiones al llegar al límite de nivel del tier de progresión actual, como en el parche 1.10. |
| `IndividualProgression.RepeatableVanillaQuestsXP` | `1` | Permite que las misiones repetibles de Vanilla concedan XP en cada entrega (comportamiento de Vanilla) en lugar de solo una vez (comportamiento de WotLK). |
| `IndividualProgression.ProgressionLimit` | `0` | Limita el tier máximo de progresión que puede alcanzar un personaje (1-17). `0` = sin límite (tier 18). |
| `IndividualProgression.StartingProgression` | `0` | Define el tier en el que empiezan los personajes nuevos/existentes. |
| `IndividualProgression.TbcRacesUnlockProgression` / `tbcRacesStartingProgression` | `0` / `0` | Tier a partir del cual se pueden crear Draenei/Elfos de Sangre, y el tier en el que empiezan. |
| `IndividualProgression.DeathKnightUnlockProgression` / `DeathKnightStartingProgression` | `13` / `13` | Tier a partir del cual se pueden crear Caballeros de la Muerte y el tier en el que empiezan. Por defecto exige completar TBC, ya que los DK no están balanceados para contenido anterior. |
| `IndividualProgression.RequiredZulGurubProgression` | `3` | Tier necesario para entrar a Zul'Gurub (por defecto: tras Ala Negra). |
| `IndividualProgression.RequiredZulAmanProgression` | `12` | Tier necesario para entrar a Zul'Aman (por defecto: tras el Templo Negro). Se sugiere `9` para abrir ZA justo después de Karazhan, en línea con el nivel de equipo previsto para esa banda. |
| `IndividualProgression.DisableDefaultProgression` | `0` | Desactiva el flujo estándar de progresión por muerte de jefes en favor de únicamente las entradas de `CustomProgression`. |
| `IndividualProgression.CustomProgression` | `""` | Pares `idCriatura tier` separados por espacio/coma para disparadores de progresión personalizados, p. ej. `"639 8, 3975 13"`. |
| `IndividualProgression.AllowEarlyDungeonSet2` | `0` | Habilita la cadena de misiones del Dungeon Set 2 antes de completar Ala Negra, para que el equipo no llegue ya obsoleto cuando se desbloquee. |
| `IndividualProgression.AllowEarlyScourgeBosses` | `0` | Habilita los 6 jefes de mazmorra de la Invasión de la Plaga fuera de su estrecha ventana del tier 6. |
| `IndividualProgression.TBC.ArenaSeason` / `WotLK.ArenaSeason` | `1` / `5` | IDs de temporada de arena activa para cada expansión. |
| `IndividualProgression.VanillaPvpKillRequirement.Rank1`-`Rank14` | ver `.conf.dist` | Umbrales de muertes con honor para cada rango/título PvP de Vanilla. |
| `IndividualProgression.VanillaPvpTitlesPersistAfterVanilla` | `1` | Conserva los títulos PvP de Vanilla obtenidos tras llegar a TBC/WotLK. |
| `IndividualProgression.VanillaPvpEarnTitlesAfterVanilla` | `0` | Permite seguir obteniendo nuevos títulos PvP de Vanilla tras dejar Vanilla. |
| `IndividualProgression.BotAccountsEarnPvPTitles` | `0` | Permite que las cuentas de bots obtengan títulos PvP. |
| `IndividualProgression.BotAccountsMaxLevel` | `80` | Nivel máximo para cuentas de bots. |
| `IndividualProgression.BotAccountsRegex` | `"^RNDBOT.*"` | Expresión regular que identifica cuentas de bots, excluidas por completo del control de progresión. |
| `IndividualProgression.ExcludedAccountsRegex` | `""` | Expresión regular para cuentas adicionales (p. ej. cuentas de prueba) a excluir del control de progresión. |
| `IndividualProgression.EnableAllSpellRanks` | `0` | Permite lanzar rangos de hechizo de Vanilla/TBC durante WotLK. Solo relevante si se ha instalado la restauración opcional de coste de maná fijo `patch-V`. |
| `IndividualProgression.EnableSetRepCommand` | `0` | Habilita `.ip setrep`, que permite copiar la reputación de un personaje de la cuenta a otro. |
| `IndividualProgression.LimitedSetRepCommand` | `1` | Exige que el personaje destino ya sea neutral/amistoso/honorado con una facción (según la facción) antes de que `.ip setrep` pueda aumentarla más — evita multiplicar la reputación mediante entregas repetidas de la misma misión. |
| `IndividualProgression.sharedFactionIdsRegex` | ver `.conf.dist` | IDs de facción admitidos para compartir reputación con `.ip setrep`. |

---

## Interruptores de jefes de Naxxramas 40

Los combates originales de Naxxramas de 40 jugadores dependían de estrategias precisas y coordinadas por humanos. Estas opciones opcionales suavizan cuatro combates para grupos (especialmente bandas con muchos bots) que no pueden ejecutar las mecánicas originales — pero no todas tienen el mismo efecto:

| Opción | Qué cambia | ¿Hace el combate superable? |
|------|------------------|----------------------------------|
| `doableNaxx40Bosses_4H` | El daño por acumulación de la Marca de Zeliek baja unas 10 veces (p. ej. 1000→100 con 3 acumulaciones); el Grito Sagrado de Zeliek baja de 443 a 222; Blaumeux deja de lanzar su Zona de Vacío. | **Sí.** La mecánica de la Marca de los Jinetes es lo que convierte este combate en un puzle de posicionamiento (cuatro tanques rotando por las esquinas); reducir su daño en un orden de magnitud lo convierte en un combate directo de tanque y golpea. Eliminar la Zona de Vacío también es clave para los bots, que no suelen salir de las zonas de daño en área hasta que ya están recibiendo daño de ellas. |
| `doableNaxx40Bosses_Gluth` | La comprobación de Gluth al comerse zombis (que lo cura un 5% de su vida máxima por zombi devorado) pasa de ejecutarse cada 3s a cada 12s. | **Sí.** Con el intervalo original de 3s, la curación por zombis suele superar al daño de la banda. Cuadruplicar el intervalo hace que la carrera de daño sea ganable. |
| `doableNaxx40Bosses_Patchwerk` | El daño de Golpe Odioso baja un ~20% (de 22100-22850 a 17680-18280). | **No por sí solo.** Golpe Odioso siempre apunta a quien tenga más vida dentro del alcance de cuerpo a cuerpo, saltándose al objetivo con más amenaza — un recorte del 20% ayuda pero no cambia esa mecánica. La solución real para bandas con bots es mantener solo al tanque en alcance de cuerpo a cuerpo: con un único objetivo posible, todos los Golpes Odiosos caen sobre el tanque, convirtiéndolo en una comprobación normal de tanque/sanador. |
| `doableNaxx40Bosses_Razuvious` | Razuvious lanza Grito Disruptivo (daño) en lugar de Combustión de Maná cada 25s. | **En gran parte.** Combustión de Maná agota el maná de los sanadores más rápido de lo que la mayoría de sanadores bot pueden recuperar, normalmente terminando el combate por agotamiento de maná en lugar de por una mala ejecución. Grito Disruptivo es daño normal que cualquier sanador puede gestionar. Esta opción *no* elimina el requisito de control mental sobre los Aprendices, que no tiene relación con la gestión de maná de los sanadores bot. |

---

## Restricción del Buscador de Mazmorras Aleatorio (RDF)

`IndividualProgression.DisableRDF = 1` bloquea el Buscador de Mazmorras del cliente (`DungeonFinder.OptionsMask` se fuerza a `4` — solo jefes estacionales — al arrancar), para que los jugadores no puedan entrar en cola a contenido fuera de su tier de progresión ni obtener la recompensa diaria/emblemas del RDF antes de tiempo.

**Esta restricción es consciente de la progresión, no un interruptor permanente.** Un enlace `GlobalScript::OnInitializeLockedDungeons` vuelve a bloquear cada mazmorra/banda no estacional para un personaje hasta que este alcanza `PROGRESSION_TBC_TIER_5` (tier 13 — desbloqueo de Naxxramas de WotLK/Ojo de la Eternidad/Santuario de Obsidiana). Esto refleja el momento real de lanzamiento del buscador: el Buscador de Mazmorras automático e inter-reinos se lanzó con el parche 3.3.0 (Ciudadela de la Corona de Hielo), bastante después de las primeras bandas de WotLK — pero restringirlo hasta el propio tier de ICC dejaría una gran parte del contenido de WotLK (Naxx 10/25, Ulduar, Prueba del Cruzado) sin ninguna herramienta de cola. El tier 13 es el punto más temprano que ya es inequívocamente "de WotLK", dando a los jugadores un buscador para el resto de la expansión.

Las cuentas de bots (identificadas por `BotAccountsRegex`) y los GMs están exentos y siempre ven las mazmorras como desbloqueadas, de forma consistente con cómo funciona el control de progresión en el resto del módulo.

Ten en cuenta que `.reload config` **no** vuelve a aplicar `DungeonFinder.OptionsMask` al sistema LFG en ejecución — la máscara de opciones solo se lee una vez, al construirse `LFGMgr`. Si necesitas cambiar la máscara del reino en caliente, usa `.lfg options <máscara>`; la restricción por tier de personaje descrita arriba funciona de forma independiente a esa máscara y se reevalúa en cada comprobación de bloqueo de mazmorra.

---

## Comandos

Todos los comandos están bajo `.ip` y requieren nivel de seguridad de GM (`SEC_GAMEMASTER`).

| Comando | Descripción |
|---------|-------------|
| `.ip get [player]` | Muestra el tier de progresión actual de un jugador. |
| `.ip set [player] <tier>` | Establece directamente el tier de progresión de un jugador. |
| `.ip setbot` | Gestión relacionada con la progresión de cuentas de bots. |
| `.ip setrep` | Copia la reputación desde el personaje con más reputación de la cuenta (ver `EnableSetRepCommand`/`LimitedSetRepCommand`). |
| `.ip pvp [player]` | Acciones relacionadas con rangos/títulos PvP. |
| `.ip tele [player] <location>` | Teletransporta a una ubicación relacionada con la progresión. |
| `.ip attune <location>` | Acciones relacionadas con la sintonización. |

---

## Discord

[https://discord.gg/PTXxh3KSgQ](https://discord.gg/PTXxh3KSgQ) (Discord del proyecto original)

---

## Créditos

* AzerothCore y TrinityCore, junto con sus predecesores Mangos.
* AzerothCore mod-war-effort
* AzerothCore mod-pvp-titles
* vMangos por los datos de progresión de Vanilla
* cMangos por los datos de TBC
* Sogladev por los scripts y datos de Naxx 40
* Módulo original: [ZhengPeiRu21/mod-individual-progression](https://github.com/ZhengPeiRu21/mod-individual-progression)

---

## Licencia

MIT
