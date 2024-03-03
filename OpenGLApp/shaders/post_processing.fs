#version 330 core
in  vec2  TexCoords;
out vec4  color;
uniform bool shake;
uniform bool dim;
uniform sampler2D scene;
uniform vec2      offsets[9];
uniform float     blur_kernel[9];


void main()
{
    //bool shake = 0;
    //color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //color =  texture(scene, TexCoords);
   
    vec4 frag = vec4(0.0);
    vec3 sample[9];
    // sample from texture offsets if using convolution matrix
    if(shake == true){
        for(int i = 0; i < 9; i++)
            sample[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
    }
    
    
    // process effect  
    if(shake == true){
        for(int i = 0; i < 9; i++)
            frag += vec4(sample[i] * blur_kernel[i], 0.0f);
        frag.a = 1.0f;
        color = frag;

    }
    if(dim == true) {
        color = vec4(texture(scene, TexCoords).rgb, 0.5);
    }    
    else{
        color = texture(scene, TexCoords);
    }
    
    
}
