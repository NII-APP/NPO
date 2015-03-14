from pyNastran.bdf.bdf import BDF

print("Xyu");
fname = input();
mesh = BDF(debug=False,log=None)
mesh.read_bdf(fname, xref=True)
print (mesh.Element(1))

print();