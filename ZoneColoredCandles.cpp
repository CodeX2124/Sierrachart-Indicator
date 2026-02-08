#include "sierrachart.h"

SCDLLName("Zone Colored Candles")

// Inputs
SCSFExport scsf_ZoneColoredCandles(SCStudyInterfaceRef sc)
{
	SCInputRef Input_Length = sc.Input[0];
    SCInputRef Input_Smoothing = sc.Input[1];

    SCInputRef Input_OB = sc.Input[2];
    SCInputRef Input_OBExtreme = sc.Input[3];
    SCInputRef Input_OS = sc.Input[4];
    SCInputRef Input_OSExtreme = sc.Input[5];
	
    if (sc.SetDefaults) {
        sc.GraphName = "Zone Colored Candles";
        sc.StudyDescription = "Zone Colored Candles similar to the TradingView indicator";

        Input_Length.Name = "Length";
        Input_Length.SetInt(14);
        Input_Length.SetIntLimits(1, INT_MAX);

        Input_Smoothing.Name = "Open Smoothing";
        Input_Smoothing.SetInt(1);
        Input_Smoothing.SetIntLimits(1, 100);

        Input_OB.Name = "OB";
        Input_OB.SetInt(20);
        Input_OB.SetIntLimits(1, 50);

        Input_OBExtreme.Name = "OB Extreme";
        Input_OBExtreme.SetInt(30);
        Input_OBExtreme.SetIntLimits(1, 50);

        Input_OS.Name = "OS";
        Input_OS.SetInt(-20);
        Input_OS.SetIntLimits(-50, -1);

        Input_OSExtreme.Name = "OS Extreme";
        Input_OSExtreme.SetInt(-30);
        Input_OSExtreme.SetIntLimits(-50, -1);

        sc.AutoLoop = 1;
		sc.GraphRegion = 1;
        return;
    }

    int length = Input_Length.GetInt();
    int smoothing = Input_Smoothing.GetInt();
    int upper = Input_OB.GetInt();
    int upperExtreme = Input_OBExtreme.GetInt();
    int lower = Input_OS.GetInt();
	int lowerExtreme = Input_OSExtreme.GetInt();
	
	// Declare the Base Data reference
    SCBaseDataRef BaseDataIn = sc.BaseData;

    // Declare subgraphs for Heikin Ashi Open, High, Low, Close
    SCSubgraphRef HeikinAshiOut = sc.Subgraph[0]; // Open prices
    HeikinAshiOut.Name = "HA Open";
    HeikinAshiOut.Arrays[0] = sc.Subgraph[1]; // High prices
    HeikinAshiOut.Arrays[1] = sc.Subgraph[2]; // Low prices
    HeikinAshiOut.Arrays[2] = sc.Subgraph[3]; // Close prices

    // Call the HeikinAshi function
    // Here we can use the auto-looping feature, so Index is handled automatically
    sc.HeikinAshi(BaseDataIn, HeikinAshiOut, sc.ArraySize, 0); // SetCloseToCurrentPriceAtLastBar is set to 0
    // Access the Heikin Ashi values
    	
	SCFloatArrayRef haOpen = HeikinAshiOut;
	SCFloatArrayRef haHigh = HeikinAshiOut.Arrays[0];
	SCFloatArrayRef haLow = HeikinAshiOut.Arrays[1];
	SCFloatArrayRef haClose = HeikinAshiOut.Arrays[2];
		
	SCSubgraphRef rsiClose = sc.Subgraph[4];
	float openRSI = 0.0f;
	float highRSI = 0.0f;
	float lowRSI = 0.0f;
	float closeRSI = 0.0f;
	
	sc.RSI(haClose, rsiClose, MOVAVGTYPE_SIMPLE, length);
	
	closeRSI = rsiClose[sc.Index] - 50.0f;
	openRSI = (sc.Index > 0) ? rsiClose[sc.Index - 1] - 50.0f : closeRSI;
	
	SCSubgraphRef rsiHigh = sc.Subgraph[5];
	sc.RSI(haHigh, rsiHigh, MOVAVGTYPE_SIMPLE, length);
	
	SCSubgraphRef rsiLow = sc.Subgraph[6];
	sc.RSI(haLow, rsiLow, MOVAVGTYPE_SIMPLE, length);
	
	float highRSIRaw = rsiHigh[sc.Index] - 50.0f;
    float lowRSIRaw = rsiLow[sc.Index] - 50.0f;
	
	highRSI = max(highRSIRaw, lowRSIRaw);
	lowRSI = min(highRSIRaw, lowRSIRaw);
	
	//Calculate open, high, low, close values
	SCFloatArrayRef close = sc.Subgraph[7].Data;
	SCFloatArrayRef open = sc.Subgraph[8].Data;
	SCFloatArrayRef high = sc.Subgraph[9].Data;
	SCFloatArrayRef low = sc.Subgraph[10].Data;
	
	close[sc.Index] = (openRSI + highRSI + lowRSI + closeRSI) / 4.0f;
	open[sc.Index] = std::isnan(open[smoothing]) ? (openRSI + closeRSI) / 2.0f : (open[1] * smoothing + close[1]) / (smoothing + 1); 
	high[sc.Index] = max(highRSI, max(open[sc.Index], close[sc.Index]));
	low[sc.Index] = min(lowRSI, min(open[sc.Index], close[sc.Index]));
	
	// Define colors
    int extra_extreme_buy_color = RGB(255, 0, 60);   // #ff003c
    int extra_extreme_sell_color = RGB(0, 30, 255);   // #001eff
    int sea_zone_upbar_color = RGB(0, 224, 64);       // #00e040
    int sea_zone_downbar_color = RGB(224, 26, 0);     // #e01a00
    int frontier_buy_sweep_color = RGB(0, 0, 0);       // #000000
    int frontier_sell_sweep_color = RGB(0, 0, 0);      // #000000
	int extreme_sell_1_color = RGB(214, 0, 51);
	
	// Define zones
    bool extra_extreme_sell = close[sc.Index] > 30;
    bool extreme_sell_1 = close[sc.Index] > 27.5f && close[sc.Index] <= 30;
    bool extreme_sell_2 = close[sc.Index] > 25 && close[sc.Index] <= 27.5f;
    bool extreme_sell_3 = close[sc.Index] > 22.5f && close[sc.Index] <= 25;
    bool extreme_sell_4 = close[sc.Index] > 20 && close[sc.Index] <= 22.5f;
    bool frontier_sell = close[sc.Index] < upperExtreme && close[sc.Index] > upper;
    bool sea_zone = close[sc.Index] < upper && close[sc.Index] > lower;
    bool frontier_buy = close[sc.Index] < lower && close[sc.Index] > lowerExtreme;
    bool extreme_buy_4 = close[sc.Index] < -20 && close[sc.Index] >= -22.5f;
    bool extreme_buy_3 = close[sc.Index] < -22.5f && close[sc.Index] >= -25;
    bool extreme_buy_2 = close[sc.Index] < -25 && close[sc.Index] >= -27.5f;
    bool extreme_buy_1 = close[sc.Index] < -27.5f && close[sc.Index] >= -30;
    bool extra_extreme_buy = close[sc.Index] < -30;
    bool fr_buy_sweep = (sea_zone && high[sc.Index] > upper);
    bool fr_sel_sweep = (sea_zone && low[sc.Index] < lower);
	
	// Determine candle color
    int candle_color;

    if (extra_extreme_sell)
        candle_color = extra_extreme_sell_color;
    else if (extreme_sell_1)
        candle_color = extreme_sell_1_color;
    else if (extreme_sell_2)
        candle_color = RGB(255, 165, 0); // Orange
    else if (extreme_sell_3)
        candle_color = RGB(255, 255, 0); // Yellow
    else if (extreme_sell_4)
        candle_color = RGB(255, 255, 255); // White
    else if (frontier_sell)
        candle_color = RGB(0, 0, 255); // Blue with 50% transparency handled separately
    else if (extra_extreme_buy)
        candle_color = extra_extreme_buy_color;
    else if (extreme_buy_1)
        candle_color = RGB(255, 0, 255); // Fuchsia
    else if (extreme_buy_2)
        candle_color = RGB(128, 0, 128); // Purple
    else if (extreme_buy_3)
        candle_color = RGB(0, 255, 255); // Aqua
    else if (extreme_buy_4)
        candle_color = RGB(255, 255, 255); // White
    else if (frontier_buy)
        candle_color = RGB(0, 255, 0); // Green with 50% transparency handled separately
    else if (fr_buy_sweep)
        candle_color = frontier_buy_sweep_color;
    else if (fr_sel_sweep)
        candle_color = frontier_sell_sweep_color;
    else
        candle_color = (close[sc.Index] > open[sc.Index]) ? sea_zone_upbar_color : sea_zone_downbar_color;
	
	// Define line properties
    COLORREF upperExtremeColor = RGB(0, 0, 255); // Blue color
    COLORREF upperColor = RGB(0, 0, 255);        // Blue color
    COLORREF lowerColor = RGB(255, 0, 0);        // Red color
    COLORREF lowerExtremeColor = RGB(255, 0, 0); // Red color

    unsigned short lineWidth = 1; // Line width
    unsigned short lineStyle = LINESTYLE_SOLID; // Solid line style
    int drawValueLabel = 1; // Draw value label
    int drawNameLabel = 1; // Do not draw name label

	sc.Subgraph[0].DrawStyle = DRAWSTYLE_COLOR_BAR;	
    sc.Subgraph[0].SecondaryColorUsed = 1;
	sc.Subgraph[0].DataColor[sc.Index] = candle_color;
}



