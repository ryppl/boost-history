a = Plot[Sqrt[Pi/(2 x)] BesselI[2.5, x], {x, -10, 10},
        PlotStyle -> Red, PlotRange -> {-10, 10}]
b = Plot[Sqrt[Pi/(2 x)] BesselK[-3.5, x], {x, -10, 10},
        PlotStyle -> Blue, PlotRange -> {-10, 10}]
c = Graphics[{Text[StyleForm[DisplayForm[RowBox[{SubscriptBox[i, 2], "(x)"}]],
                   FontSize -> 14], {5.5, 5}],
              Text[StyleForm[DisplayForm[RowBox[{SubscriptBox[k, -4], "(x)"}]],
                   FontSize -> 14], {-5, 5}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["smbessel_n.png", d, ImageSize -> {600, 400}]