<Qucs Schematic 4.4.0>
<Properties>
  <View=0,0,800,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=python_function_optimization.dat>
  <DataDisplay=python_function_optimization.dpl>
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
  <multi.Loadcase lc1 1 600 180 -51 29 0 0 "fake_multi_test.psl" 0 "" 0 "X1,V1,A1" 0 "PRADIS_Dynamic" 0 "{" 0 "}" 0>
  <multi.Variable A 1 100 180 -51 29 0 0 "A" 0 "1.3" 0 "-2" 0 "2" 0>
  <multi.Variable B 1 190 180 -51 29 0 0 "B" 0 "0.7" 0 "-2" 0 "2" 0>
  <multi.Optimization Optimization1 1 260 280 -71 29 0 0 "NelderMead" 0 "A,B" 0 "lc1" 0 "UserFunction" 0 "" 0 "1e-4" 0 "1e-3" 0 "100" 0 "1e8" 0 "4" 0>
  <Data UserFunction1 1 170 80 -38 21 0 0 "def UserFunction (x, ma)%dp;\n  v1 = ( 1.0- x[0])**2+100.0* (x[1]-x[0]**2)**2\n  v = v1\n\n  return v\n\ndef cf1(x, ma)%dp;\n  v = -1.0\n  return v" 0>
</Components>
<Wires>
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
