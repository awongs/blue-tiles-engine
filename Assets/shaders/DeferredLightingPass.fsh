#version 330 core

// Structure that holds directional light variables.
struct DirLight {
	float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;

    vec3 colour;
    vec3 direction;
};

// Structure that holds point light variables.
struct PointLight {
	float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;

    vec3 colour;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
};

// Structure that holds spot light variables
struct SpotLight {
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;

    vec3 colour;
    vec3 position;
    vec3 direction;
    
    float innerRadius;
    float outerRadius;
    
    float constant;
    float linear;
    float quadratic;
};

layout (location = 0) out vec4 fragColour;

// Maximum number of lights that can affect fragment are defined here.
#define MAX_POINT_LIGHTS 64
#define MAX_SPOT_LIGHTS 32

// Lighting uniforms
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform DirLight dirLight;

uniform int totalPointLights;
uniform int totalSpotLights;

// Position of the camera
uniform vec3 cameraPosition; 

// G-Buffer uniforms
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;

// Input from vertex shader
in vec2 fragTexCoord;

// Forward declarations
vec4 calcDirLighting(vec3 normal, vec3 viewDir);
vec4 calcPointLighting(PointLight pointLight, vec3 normal, vec3 fragPosition, vec3 viewDir);
vec4 calcSpotLighting(SpotLight spotLight, vec3 normal, vec3 fragPosition, vec3 viewDir);

void main()
{
	// Sample geometry from textures
	vec3 position = texture(gPosition, fragTexCoord).xyz;
	vec3 normal = normalize(texture(gNormal, fragTexCoord).xyz);
	vec4 colour = texture(gColour, fragTexCoord);

	// Calculate view direction
	vec3 viewDir = normalize(cameraPosition - position);

	// Calculate directional lighting
    vec4 lighting = calcDirLighting(normal, viewDir);

    // Calculate point lighting
    for (int i = 0; i < totalPointLights; i++) {
        lighting += calcPointLighting(pointLights[i], normal, position, viewDir);
    }

    // Calculate spot lighting
    for (int i = 0; i < totalSpotLights; i++) {
        lighting += calcSpotLighting(spotLights[i], normal, position, viewDir);
    }

	fragColour = colour * lighting;
}

/**
 * Calculates and returns the effect that directional lighting will have on this fragment.
 * normal - the normal vector of this fragment
 */
vec4 calcDirLighting(vec3 normal, vec3 viewDir) {
    // Ambient
    vec3 ambientColour = dirLight.colour * dirLight.ambientIntensity;
    vec3 direction = normalize(dirLight.direction);
    
    // Diffuse
    float diffuseFactor = max(dot(normal, -direction), 0.0);
    vec3 diffuseColour = dirLight.colour * dirLight.diffuseIntensity * diffuseFactor;
    
    // Specular
    vec3 reflection = reflect(-direction, normal);
    float specularFactor = pow(max(0.0, dot(reflection, viewDir)), 16.0);
    vec3 specularColour = dirLight.colour * dirLight.specularIntensity * specularFactor;
    
    // Add all three for phong lighting
    return vec4((ambientColour + diffuseColour + specularColour), 1.0);
}

/**
 * Calculates and returns the effect that point lighting will have on this fragment.
 * pointLight - the point light being calculated
 * normal - the normal vector of this fragment
 * fragPosition - the position of the fragment
 * viewDir - the viewing direction from the camera to this fragment
 */
vec4 calcPointLighting(PointLight pointLight, vec3 normal, vec3 fragPosition, vec3 viewDir) {
    // Ambient
    vec3 ambientColour = pointLight.colour * pointLight.ambientIntensity;
    
    vec3 lightDir = normalize(fragPosition - pointLight.position);
    
    // Diffuse
    float diffuseFactor = max(dot(normal, -lightDir), 0.0);
    vec3 diffuseColour = pointLight.colour * pointLight.diffuseIntensity * diffuseFactor;
    
    // Specular
    vec3 reflection = reflect(lightDir, normal);
    float specularFactor = pow(max(dot(reflection, viewDir), 0.0), 16.0);
    vec3 specularColour = pointLight.colour * pointLight.specularIntensity * specularFactor;
    
    // Point light attenuation
    float dist = length(pointLight.position - fragPosition);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist +
                               pointLight.quadratic * (dist * dist));
    
    // Combine results
    return vec4((ambientColour + diffuseColour + specularColour) * attenuation, 1.0);
}

/**
 * Calculates and returns the effect that spot lighting will have on this fragment.
 * pointLight - the point light being calculated
 * normal - the normal vector of this fragment
 * fragPosition - the position of the fragment
 * viewDir - the viewing direction from the camera to this fragment
 */
lowp vec4 calcSpotLighting(SpotLight spotLight, vec3 normal, vec3 fragPosition, vec3 viewDir) {
    
    // Vector from the light source to the fragment
    lowp vec3 fragDirection = fragPosition - spotLight.position;
    lowp float fragDistance = length(fragDirection);
    
    // Normalize the fragment direction
    fragDirection = normalize(fragDirection);
    
    lowp float attenuation = 1.0 / (spotLight.constant + spotLight.linear * fragDistance +
                                    spotLight.quadratic * (fragDistance * fragDistance));
    
    lowp float theta = acos(dot(fragDirection, spotLight.direction));
    lowp float epsilon = spotLight.innerRadius - spotLight.outerRadius;
    lowp float intensity = clamp((theta - spotLight.outerRadius) / epsilon, 0.0, 1.0) * attenuation;
    
    // Ambient
    lowp vec3 ambientColour = spotLight.colour * spotLight.ambientIntensity;
    
    // Diffuse
    lowp float diffuseFactor = max(dot(normal, -fragDirection), 0.0);
    lowp vec3 diffuseColour = spotLight.colour * spotLight.diffuseIntensity * diffuseFactor;
    
    // Specular
    lowp vec3 reflection = reflect(fragDirection, normal);
    lowp float specularFactor = pow(max(dot(reflection, viewDir), 0.0), 16.0);
    lowp vec3 specularColour = spotLight.colour * spotLight.specularIntensity * specularFactor;
    
    // Combine results
    return vec4((ambientColour + diffuseColour + specularColour) * vec3(intensity), 1.0);
}