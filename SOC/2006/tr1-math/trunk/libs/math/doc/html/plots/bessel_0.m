a = Plot[BesselJ[0, x], {x, -10, 10}, PlotStyle -> Red]
b = Plot[BesselY[0, x], {x, 0, 10}, PlotStyle -> Blue]
c = Graphics[{Text[TraditionalForm[BesselJ[0, x]], {1.4, 1}],
              Text[TraditionalForm[BesselY[0, x]], {3.5, 0.55}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["bessel_0.png", d, ImageSize -> {600, 400}]