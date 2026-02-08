# 🧭 Zone Colored Candles 

A Sierra Chart custom study that colors candles by RSI-based zones (overbought/oversold, extremes, and frontier sweeps), inspired by the TradingView Zone Colored Candles indicator—built for forex and other markets.

---

## 📚 Table of Contents

- [About](#about)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Screenshots](#screenshots)
- [API Documentation](#api-documentation)
- [Contact](#contact)
- [Acknowledgements](#acknowledgements)

---

## 🧩 About

This project provides a **Zone Colored Candles** study for Sierra Chart, giving traders a visual view of momentum and zones (overbought/oversold and extremes) directly on the chart. Candles are colored by a Heikin-Ashi–based RSI-style calculation, with configurable OB/OS levels and customizable colors for each zone. The goal is to bring a TradingView-style zone-colored view into Sierra Chart for use in forex and other trading workflows.

---

## ✨ Features

- **Zone-based candle coloring** – Candles colored by RSI-derived zones (sea zone, frontier buy/sell, extreme buy/sell levels).
- **Heikin-Ashi + RSI logic** – Uses Heikin-Ashi OHLC and RSI (centered around 50) for smooth, zone-based coloring.
- **Configurable OB/OS levels** – Adjustable OB, OB Extreme, OS, and OS Extreme thresholds (e.g. 20/30 and -20/-30).
- **Customizable colors** – Per-zone color inputs for extreme buy/sell, sea zone bars, frontier sweeps, and frontier buy/sell.
- **Length & smoothing** – RSI length and open smoothing parameters for tuning sensitivity.
- **64-bit DLL** – Pre-built `ZoneColoredCandles_64.dll` for Sierra Chart 64-bit.

---

## 🧠 Tech Stack

| Category   | Details                                      |
|-----------|-----------------------------------------------|
| **Languages**  | C++                                          |
| **Platform**   | Sierra Chart (Windows)                       |
| **Build**      | Sierra Chart ACSIL / Visual Studio (64-bit)  |
| **Output**     | Native 64-bit DLL (ZoneColoredCandles_64.dll)|

---

## ⚙️ Installation

1. **Get the study files**
   - Clone or download this repository, or copy the study DLL and (optionally) source.
   ```bash
   git clone https://github.com/CodeX2124/Sierrachart-Indicator.git
   ```

2. **Install the DLL in Sierra Chart**
   - Copy `ZoneColoredCandles_64.dll` into your Sierra Chart **Custom Studies (64-bit)** folder, for example:
   - `Sierra Chart/ACS_Source/Custom Studies (64-bit)/`
   - Or the folder set in Sierra Chart: **Global Settings → General → Custom Study DLL Directory (64-bit)**.

3. **Restart Sierra Chart** (if it was running) so it loads the new DLL.

4. **(Optional) Build from source**
   - Open the project in Visual Studio (or your Sierra Chart ACSIL setup).
   - Build a 64-bit Release DLL and place the output in the Custom Studies folder as above.

---

## 🚀 Usage

1. Open a chart in Sierra Chart (e.g. forex or any instrument).
2. **Chart → Study/Price Overlay → Add Study…**
3. Find **Zone Colored Candles** in the list and add it.
4. Adjust **Length**, **Open Smoothing**, **OB**, **OB Extreme**, **OS**, **OS Extreme** and any **color** inputs as needed.
5. Candles will be colored by the current zone (sea zone, frontier, extreme buy/sell, sweeps).

No web server or URL—everything runs inside Sierra Chart.

---

## 🧾 Configuration

Study inputs (set in Sierra Chart study settings):

| Input            | Description              | Example |
|------------------|--------------------------|---------|
| **Length**       | RSI period               | 14      |
| **Open Smoothing** | Smoothing for “open”   | 1       |
| **OB**           | Overbought level         | 20      |
| **OB Extreme**   | Extreme overbought      | 30      |
| **OS**           | Oversold level           | -20     |
| **OS Extreme**   | Extreme oversold        | -30     |
| **Color inputs** | Per-zone colors          | RGB values (see study dialog) |

No `.env` or config file is required; all settings are in the study’s **Settings** window in Sierra Chart.

## 📜 API Documentation

This project is a Sierra Chart **custom study** (ACSIL DLL). It does not expose a REST or other external API. The “interface” is:

- **Inputs:** Study inputs (Length, Smoothing, OB/OS, colors) in the Sierra Chart study settings.
- **Output:** Candle coloring (draw style `DRAWSTYLE_COLOR_BAR`) on the chart.

For Sierra Chart ACSIL reference, see the official Sierra Chart documentation and ACSIL headers.

---

## 📬 Contact

- **Email:** codex199201@gmail.com 
- **GitHub:** CodeX2124   

---

## 🌟 Acknowledgements

- **Sierra Chart** – Trading platform and ACSIL API for custom studies.
- **TradingView** – Inspiration for the Zone Colored Candles concept.
- Heikin-Ashi and RSI implementations follow standard definitions; zone thresholds and colors are adapted for this study.
