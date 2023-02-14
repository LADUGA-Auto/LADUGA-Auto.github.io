<Qucs Schematic 4.4.0>
<Properties>
  <View=0,0,800,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=tr7_OPT.dat>
  <DataDisplay=tr7_OPT.dpl>
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
  <multi.Variable N1 1 110 140 -51 29 0 0 "N1" 0 "10" 0 "1" 0 "20" 0>
  <multi.Variable N2 1 210 140 -51 29 0 0 "N2" 0 "8" 0 "1" 0 "20" 0>
  <multi.Variable N3 1 300 140 -51 29 0 0 "N3" 0 "6" 0 "1" 0 "20" 0>
  <multi.Variable N4 1 390 140 -51 29 0 0 "N4" 0 "5" 0 "1" 0 "20" 0>
  <multi.Variable N5 1 480 140 -51 29 0 0 "N5" 0 "4" 0 "1" 0 "20" 0>
  <multi.Variable T3 1 300 230 -51 29 0 0 "T3" 0 "6" 0 "1" 0 "20" 0>
  <multi.Variable T4 1 390 230 -51 29 0 0 "T4" 0 "9" 0 "1" 0 "20" 0>
  <multi.Variable T5 1 480 230 -51 29 0 0 "T5" 0 "13" 0 "1" 0 "20" 0>
  <multi.Variable T1 1 110 230 -51 29 0 0 "T1" 0 "0.1" 0 "0.001" 0 "20" 0>
  <Data UserFunction1 1 280 40 -38 21 0 0 "def UserFunction (x, ma)%dp;\n  v1 =  ma.f('lc1.Time60')\n  v2 =  ma.f('lc1.Time100')\n  v3 =  ma.f('lc1.Velocity')\n  v = - v3 + 20 * v2 + (v2-v1)*10\n\n  return v\n\ndef cf1(x, ma)%dp;\n  v = x[5]-x[6]+0.001\n  return v\n\ndef cf2(x, ma)%dp;\n  v = x[6]-x[7]+0.001\n  return v\n\ndef cf3(x, ma)%dp;\n  v = x[7]-x[8]+0.001\n  return v\n\ndef cf4(x, ma)%dp;\n  v = x[8]-x[9]+0.001\n  return v\n\n\ndef cf5(x, ma)%dp;\n  v =  ma.f('lc1.MaxTorque') - 10000\n  return v" 0>
  <multi.Variable T2 1 220 230 -51 29 0 0 "T2" 0 "4" 0 "1" 0 "20" 0>
  <multi.Optimization Optimization1 1 130 340 -71 29 0 0 "NelderMead" 0 "N1,N2,N3,N4,N5,T1,T2,T3,T4,T5" 0 "lc1" 0 "UserFunction" 0 "cf1,cf2,cf3,cf4,cf5" 0 "1e-2" 0 "1e-1" 0 "100" 0 "1e6" 0 "4" 0>
  <multi.Loadcase lc1 1 110 60 -51 29 0 0 "tr7_MULTI_2.sch.psl" 0 "" 0 "Time60,Time100,Velocity,MaxTorque" 0 "PRADIS_Dynamic" 0 "{" 0 "}" 0>
</Components>
<Wires>
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
