a = Plot[BesselI[0, x], {x, -3, 3}, PlotStyle -> Red]
b = Plot[BesselK[0, x], {x, 0, 3}, PlotStyle -> Blue]
c = Graphics[{Text[TraditionalForm[BesselI[0, x]], {-2, 1.5}],
              Text[TraditionalForm[BesselK[0, x]], {2, 0.5}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["mbessel_0.png", d, ImageSize -> {600, 400}]