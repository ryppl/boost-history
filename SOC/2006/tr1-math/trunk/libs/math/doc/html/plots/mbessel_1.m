a = Plot[BesselI[1, x], {x, -6, 6}, PlotStyle -> Red, PlotRange -> {-20,20}]
b = Plot[BesselK[1, x], {x, 0, 6}, PlotStyle -> Blue, PlotRange -> {-20,20}]
c = Graphics[{Text[TraditionalForm[BesselI[1, x]], {4.5, 10}],
              Text[TraditionalForm[BesselK[1, x]], {0.5, 10}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["mbessel_1.png", d, ImageSize -> {600, 400}]