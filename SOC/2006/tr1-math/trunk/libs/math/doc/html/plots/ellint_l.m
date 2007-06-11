a = Plot[EllipticK[x^2], {x, 0, 1}, PlotStyle -> Red, PlotRange -> {1, 5}]
b = Plot[EllipticE[x^2], {x, 0, 1}, PlotStyle -> Blue, PlotRange -> {1, 5}]
c = Plot[EllipticPi[0.5, x^2], {x, 0, 1}, PlotStyle -> Green, PlotRange -> {1, 5}]
d = Graphics[{Text[StyleForm[TraditionalForm[EllipticK[k]],
                   FontSize -> 14], {0.9, 2.5}],
              Text[StyleForm[TraditionalForm[EllipticE[k]],
                   FontSize -> 14], {0.6, 1.3}],
              Text[StyleForm[TraditionalForm[EllipticPi[0.5, k]],
                   FontSize -> 14], {0.6, 2.8}]}]
e = Show[a, b, c, d]
SetDirectory["D://"]
Export["ellint_l.png", e, ImageSize -> {600, 400}]