from pyNastran.bdf.bdf import BDF
import sys
import numpy
write = sys.stdout.buffer.write
writeInt = lambda val: write(val.to_bytes(4, sys.byteorder))
fname = 'C:\\Users\\BOPOHOB\\Documents\\Qt\\work\\NPO\\Aaron.bdf'
mesh = BDF(debug=False,log=None)
mesh.read_bdf(input("read") if 'fname' not in locals() else fname, xref=True)
a = imput("readed:\n")
print(a)

#store the vertexes
m=numpy.zeros(max(mesh.nodes.keys()) * 3 + 3)
for i, node in mesh.nodes.items():
    m[i * 3 : i * 3 + 3] = node.get_position()
write(bytes(len(m)))
write(m.tobytes())

#store the elements
for i, element in mesh.elements.items():
    writeInt(i)
    if element.type.upper() == 'CQUAD4':
        assert len(element.nodeIDs()) == 4
        writeInt(1)
    elif element.type.upper() == 'QTETRA3':
        assert len(element.nodeIDs()) == 4
        writeInt(2)
    elif element.type.upper() == 'CTRIA3':
        assert len(element.nodeIDs()) == 3
        writeInt(4)
    elif element.type.upper() == 'CBAR':
        writeInt(5)
    elif element.type.upper() == 'CBUSH':
        writeInt(6)
    else:
        print('unnown element type {}'.format(element.type))
        assert 'convertor fail'
    writeInt(element.Pid())
    writeInt(len(element.nodeIDs()))
    write(numpy.array(element.nodeIDs(), dtype=numpy.int).tobytes())
