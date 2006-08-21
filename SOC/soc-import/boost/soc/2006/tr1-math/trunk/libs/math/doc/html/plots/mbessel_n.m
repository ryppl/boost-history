a = Plot[BesselI[3, x], {x, -6, 6}, PlotStyle -> Red, PlotRange -> {-20,20}]
b = Plot[BesselK[2, x], {x, 0, 6}, PlotStyle -> Blue, PlotRange -> {-20,20}]
c = Graphics[{Text[TraditionalForm[BesselI[3, x]], {5.5, 10}],
              Text[TraditionalForm[BesselK[2, x]], {1, 10}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["mbessel_n.png", d, ImageSize -> {600, 400}]