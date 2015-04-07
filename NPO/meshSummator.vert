in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Form;
uniform float k;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(in_Position + in_Form * k, 1.0);
    if (gl_Color.x != 0 || gl_Color.y != 0 || gl_Color.z != 0) {
        gl_FrontColor = gl_Color;
    } else {
        gl_FrontColor = vec4(in_Color, 1.0);
    } 
}
