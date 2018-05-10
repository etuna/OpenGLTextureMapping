varying  vec4 color;
varying float porg;

uniform sampler2D texture;
varying  vec3 fN;
varying  vec3 fL;
varying  vec3 fV;

varying  vec3 fN2;
varying  vec3 fL2;
varying  vec3 fV2;

varying vec2 texCoord;
varying float mlights;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
varying vec4 Light2Position;
uniform float Shininess;
varying float shading;
varying float material;


void main() 
{ 


if(porg == 0.0){

if(mlights == 0.0){



 // Normalize the input lighting vectors
        vec3 N = normalize(fN);
        vec3 V = normalize(fV);
        vec3 L = normalize(fL);

        vec3 H = normalize( L + V );
        
        vec4 ambient = AmbientProduct;

        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;
        
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks*material*SpecularProduct;

        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        gl_FragColor = ambient;
        gl_FragColor.a = 1.0;




} else if(mlights == 1.0){

 // Normalize the input lighting vectors
        vec3 N = normalize(fN);
        vec3 V = normalize(fV);
        vec3 L = normalize(fL);

        vec3 H = normalize( L + V );
        
        vec4 ambient = AmbientProduct;

        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;
        
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks*material*SpecularProduct;

        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        gl_FragColor = ambient + diffuse + specular;
        gl_FragColor.a = 1.0;






		} else {//two lights with phong
		vec3 N = normalize(fN);
        vec3 V = normalize(fV);
        vec3 L = normalize(fL);

        vec3 H = normalize( L + V );
        
        vec4 ambient = AmbientProduct;

        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;
        
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks*material*SpecularProduct;

        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        gl_FragColor = ambient + diffuse + specular;
        gl_FragColor.a = 1.0;

		



		N = normalize(fN2);
        V = normalize(fV2);
        L = normalize(fL2);

        H = normalize( L + V );
        
        ambient = AmbientProduct;

        Kd = max(dot(L, N), 0.0);
        diffuse += Kd*DiffuseProduct;
        
        Ks = pow(max(dot(N, H), 0.0), Shininess);
        specular += Ks*material*SpecularProduct;

        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        gl_FragColor = ambient + diffuse + specular;
        gl_FragColor.a = 1.0;
		
		
		
		
		}




















}else {


if(shading == 1.0){
    gl_FragColor = color;

}else if(shading == 2.0){//texture

gl_FragColor = texture2D( texture, texCoord );


}else {
gl_FragColor = color;
}

} 
}

