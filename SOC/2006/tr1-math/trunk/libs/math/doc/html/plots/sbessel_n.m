a = Plot[Sqrt[Pi/(2 x)] BesselJ[2.5, x], {x, -10, 10},
        PlotStyle -> Red, PlotRange -> {-2, 1}]
b = Plot[Sqrt[Pi/(2 x)] BesselY[2.5, x], {x, -10, 10},
        PlotStyle -> Blue, PlotRange -> {-2, 1}]
c = Graphics[{Text[StyleForm[DisplayForm[RowBox[{SubscriptBox[j, 2], "(x)"}]],
                   FontSize -> 14], {3.4, 0.4}],
              Text[StyleForm[DisplayForm[RowBox[{SubscriptBox[y, 2], "(x)"}]],
                   FontSize -> 14], {2.7, -1}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["sbessel_n.png", d, ImageSize -> {600, 400}]