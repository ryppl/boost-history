a = Plot[BesselJ[1, x], {x, -10, 10}, PlotStyle -> Red]
b = Plot[BesselY[1, x], {x, 0, 10}, PlotStyle -> Blue]
c = Graphics[{Text[TraditionalForm[BesselJ[1, x]], {1.4, 0.7}],
              Text[TraditionalForm[BesselY[1, x]], {4, 0.5}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["bessel_1.png", d, ImageSize -> {600, 400}]