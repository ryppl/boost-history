a = Plot[x^(-0.5), {x, 0, 4}, PlotStyle -> Red, PlotRange -> {0, 10}]
b = Plot[x^(-1.5), {x, 0, 4}, PlotStyle -> Blue, PlotRange -> {0, 10}]
c = Graphics[{Text[StyleForm[DisplayForm[RowBox[{SubscriptBox[R, F], "(x,x,x)"}]], 
                   FontSize -> 14], {0.4, 1}],
              Text[StyleForm[DisplayForm[RowBox[{SubscriptBox[R, J], "(x,x,x,x)"}]],
                   FontSize -> 14], {0.8, 4}]}]
d = Show[a, b, c]
SetDirectory["D://"]
Export["ellint_c.png", d, ImageSize -> {600, 400}]