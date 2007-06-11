a = Plot[BesselI[Sqrt[5], x], {x, 0, 6}, PlotStyle -> Red, PlotRange -> {0, 20}]
b = Plot[BesselK[-Pi/2, x], {x, 0, 6}, PlotStyle -> Blue, PlotRange -> {0, 20}]
c = Graphics[{Text[StyleForm[TraditionalForm[BesselI[Sqrt[5], x]],
                   FontSize -> 14], {5.2, 10}],
              Text[StyleForm[TraditionalForm[BesselK[-Pi/2, x]],
                   FontSize -> 14], {0.8, 10}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["mbessel_v.png", d, ImageSize -> {600, 400}]