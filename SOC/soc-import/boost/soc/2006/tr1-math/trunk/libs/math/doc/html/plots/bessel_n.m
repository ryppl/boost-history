a = Plot[BesselJ[3, x], {x, -10, 10}, PlotStyle -> Red]
b = Plot[BesselY[2, x], {x, 0, 10}, PlotStyle -> Blue]
c = Graphics[{Text[TraditionalForm[BesselJ[3, x]], {3.5, 0.55}],
              Text[TraditionalForm[BesselY[2, x]], {2.5, -1}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["bessel_n.png", d, ImageSize -> {600, 400}]