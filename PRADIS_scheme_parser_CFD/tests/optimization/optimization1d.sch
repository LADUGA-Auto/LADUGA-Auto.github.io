<Qucs Schematic 4.4.0>
<Properties>
  <View=0,0,800,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=optimization1d.dat>
  <DataDisplay=optimization1d.dpl>
  <OpenDisplay=1>
  <showFrame=0>
  <FrameText0=\x041D\x0430\x0437\x0432\x0430\x043D\x0438\x0435>
  <FrameText1=\x0427\x0435\x0440\x0442\x0438\x043B:>
  <FrameText2=\x0414\x0430\x0442\x0430:>
  <FrameText3=\x0412\x0435\x0440\x0441\x0438\x044F:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Data UserFunction1 1 190 80 -38 21 0 0 "def UserFunction (x, ma)%dp;\n  v1 = ( ma.f('lc1.X1') -0.5)**2\n  v = v1\n\n  return v\n\ndef cf1(x, ma)%dp;\n  v = 0.6 - ma.f('lc1.X1')\n  return v" 0>
  <multi.Loadcase lc2 1 490 180 -51 29 0 0 "multi_test1D3.psl" 0 "" 0 "X1,V1,A1" 0 "PRADIS_Dynamic" 0 "{" 0 "}" 0>
  <multi.Loadcase lc1 1 600 180 -51 29 0 0 "multi_test1d.psl" 0 "" 0 "X1,V1,A1" 0 "PRADIS_Dynamic" 0 "{" 0 "}" 0>
  <multi.Variable A 1 120 180 -51 29 0 0 "A" 0 "500" 0 "1" 0 "1000" 0>
  <multi.Optimization Optimization1d 1 280 290 -71 29 0 0 "Opt_1D" 0 "A" 0 "lc1, lc2" 0 "UserFunction" 0 "cf1" 0 "1e-2" 0 "1e-2" 0 "100" 0 "1e8" 0 "2" 0>
</Components>
<Wires>
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
