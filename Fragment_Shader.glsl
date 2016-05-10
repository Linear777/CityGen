layout(location = 0) out vec4 out_color;

in vec4 color;

float LinearizeDepth(float depth)
{
    float near = 0.1; 
    float far = 1000.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near) / (far + near - z * (far - near));	
}

void main(void)
{
	float depth = 0.5 +LinearizeDepth(gl_FragCoord.z);
    out_color = vec4(depth);
}


