a = Plot[BesselJ[Sqrt[5], x], {x, 0, 20}, PlotStyle -> Red, PlotRange -> {-1, 0.75}]
b = Plot[BesselY[-Pi/2, x], {x, 0, 20}, PlotStyle -> Blue, PlotRange -> {-1, 0.75}]
c = Graphics[{Text[StyleForm[TraditionalForm[BesselJ[Sqrt[5], x]],
                   FontSize -> 14], {3.5, 0.55}],
              Text[StyleForm[TraditionalForm[BesselY[-Pi/2, x]],
                   FontSize -> 14], {4.5, -0.5}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["bessel_v.png", d, ImageSize -> {600, 400}]