#version 330

in vec4 fragmentColor;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
    float matRoughness;
    float matSpecular;
    float matMetallic;
} material;

struct LightProperties {
    vec4 diffuseColor;
    vec4 specularColor;
};

struct EpicLightProperties {
    float light_radius;
    vec4 cLight;
    vec4 sky_color;
    vec4 ground_color;
    vec4 point_position;
    vec4 forward_direction;
};

uniform LightProperties genericLight;

struct PointLight {
    vec4 pointPosition;
};
uniform PointLight pointLight;
uniform EpicLightProperties directionalLight;
uniform EpicLightProperties hemisphereLight;


uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;

float calculateG(vec4 direction,vec4 N,float k){
    
    float G = clamp(dot(N,direction),0,1)/(clamp(dot(N,direction),0,1)*(1-k)+k);
    
    return G;
    
}

vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    
    vec4 cBase = fragmentColor;
    
    vec4 cDiff = (1-material.matMetallic)*cBase;
    
    vec4 matSpecular2 = vec4(material.matSpecular,material.matSpecular,material.matSpecular,material.matSpecular);
    
    vec4 cSpec = mix(0.08*matSpecular2,cBase,material.matMetallic);
    
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = normalize(pointLight.pointPosition - worldPosition);

    // Direction from the surface to the eye
    vec4 V = normalize(cameraPosition - worldPosition);
    
    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + V);
    
    float alpha = pow(material.matRoughness,2);
    
    float numerator = pow(clamp(dot(N,H),0,1),2) * (pow(alpha,2) - 1) + 1;
    
    float D = (pow(alpha,2))/(3.14159265358979323846 * pow(numerator,2));
    
    float k = pow(material.matRoughness + 1,2)/8;

    float G = calculateG(L,N,k)*calculateG(V,N,k);
    
    float twoPow =(-5.55473 * clamp(dot(V,H),0,1)- 6.98316)* clamp(dot(V,H),0,1);

    vec4 F = cSpec + (1-cSpec)*pow(2,twoPow);
    
    //final values of d and s
                              
    float d = float(cDiff/3.14159265358979323846);
    
    float s = float((D * F * G)/ (4*clamp(dot(N,L),0,1)*clamp(dot(N,V),0,1)));
    
    vec4 cLight = vec4(1.f,1.f,1.f,1.f);
                              
    vec4 cFinal = cLight*clamp(dot(N,L),0,1)*(d+s);

    return cFinal;
    
}

vec4 directionalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    vec4 L = -directionalLight.forward_direction;
    
    vec4 cLight = directionalLight.cLight; //light color
    
    vec4 cBase = fragmentColor;
    
    vec4 cDiff = (1-material.matMetallic)*cBase;
    
    vec4 matSpecular2 = vec4(material.matSpecular,material.matSpecular,material.matSpecular,material.matSpecular);
    
    vec4 cSpec = mix(0.08*matSpecular2,cBase,material.matMetallic);
    
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the eye
    vec4 V = normalize(cameraPosition - worldPosition);
    
    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + V);
    
    float alpha = pow(material.matRoughness,2);
    
    float numerator = pow(clamp(dot(N,H),0,1),2) * (pow(alpha,2) - 1) + 1;
    
    float D = (pow(alpha,2))/(3.14159265358979323846 * pow(numerator,2));
    
    float k = pow(material.matRoughness + 1,2)/8;
    
    float G = calculateG(L,N,k)*calculateG(V,N,k);
    
    float twoPow =(-5.55473 * clamp(dot(V,H),0,1)- 6.98316)* clamp(dot(V,H),0,1);
    
    vec4 F = cSpec + (1-cSpec)*pow(2,twoPow);
    
    //final values of d and s
    
    float d = float(cDiff/3.14159265358979323846);
    
    float s = float((D * F * G)/ (4*clamp(dot(N,L),0,1)*clamp(dot(N,V),0,1)));
    
    vec4 cFinal = cLight*clamp(dot(N,L),0,1)*(d+s);
    
    return cFinal;

}


vec4 hemisphereLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    
    vec3 N = normalize(worldNormal);
    
    float interp_val = clamp(dot(N, vec3(0.f, 1.f, 0.f))*0.5 + 0.5f, 0.f, 1.f);
    
    vec4 cSky = hemisphereLight.sky_color; //sky color
    
    vec4 cGround = hemisphereLight.ground_color; //ground color
    
    vec4 cLight = mix(cGround, cSky, interp_val);

    vec4 L = vec4(N, 0.f);
    
    vec4 cBase = fragmentColor;
    
    vec4 cDiff = (1-material.matMetallic)*cBase;
    
    vec4 matSpecular2 = vec4(material.matSpecular,material.matSpecular,material.matSpecular,material.matSpecular);
    
    vec4 cSpec = mix(0.08*matSpecular2,cBase,material.matMetallic);
    
    // Direction from the surface to the eye
    vec4 V = normalize(cameraPosition - worldPosition);
    
    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + V);
    
    float alpha = pow(material.matRoughness,2);
    
    float numerator = pow(clamp(dot(L,L),0,1),2) * (pow(alpha,2) - 1) + 1;
    
    float D = (pow(alpha,2))/(3.14159265358979323846 * pow(numerator,2));
    
    float k = pow(material.matRoughness + 1,2)/8;
    
    float G = calculateG(L,L,k)*calculateG(V,L,k);
    
    float twoPow =(-5.55473 * clamp(dot(V,H),0,1)- 6.98316)* clamp(dot(V,H),0,1);
    
    vec4 F = cSpec + (1-cSpec)*pow(2,twoPow);
    
    //final values of d and s
    
    float d = float(cDiff/3.14159265358979323846);
    
    float s = float((D * F * G)/ 4*dot(L,L)*dot(L,V));
    
    vec4 cFinal = cLight*clamp(dot(L,L),0,1)*(d+s);
    
    return cFinal;
}


vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0);
}

vec4 AttenuateLight(vec4 originalColor, vec4 worldPosition)
{
    float lightDistance = length(pointLight.pointPosition - worldPosition);
    
    float numerator2 = clamp(1-pow(lightDistance/1000,4),0,1);
    
    float attenuation = float((numerator2 * numerator2) / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation));
    return originalColor * attenuation;
}

void main()
{
    vec4 lightingColor = vec4(0);
    if (lightingType == 0) {
        lightingColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 1) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 2) {
        lightingColor = directionalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 3) {
        lightingColor = hemisphereLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    }
    
    finalColor = AttenuateLight(lightingColor, vertexWorldPosition) * fragmentColor;
}
