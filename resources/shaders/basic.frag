#version 450 core

in vec3 vertNormal;
in vec3 vertColor;
in vec3 fragPosition;

out vec4 FragColor;

uniform vec3 viewPosition;

uniform bool lightable;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform int numberOfDirectionalLights;
uniform struct DirectionalLight {
    vec3 direction;
    vec3 color;

    float ambientStrength;
} directionalLights[32];

uniform int numberOfPointLights;
uniform struct PointLight {
    vec3 position;
    vec3 color;

    float radius;
    float ambientStrength;
} pointLights[32];

uniform int numberOfSpotLights;
uniform struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 color;

    float range;
    float angle;
    float blurAngle;
} spotLights[32];

vec3 calculateDirectionalLight(DirectionalLight light, Material material, vec3 vertNormal, vec3 viewPosition) {
    float diff = max(dot(vertNormal, light.direction), 0.0);

    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-light.direction, vertNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    vec3 ambient = light.color * light.ambientStrength * material.ambient;
    vec3 diffuse = light.color * (diff * material.diffuse);
    vec3 specular = light.color * (spec * material.specular);

    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, Material material, vec3 vertNormal, vec3 fragPosition, vec3 viewPosition) {
    vec3 lightDir = normalize(light.position - fragPosition);
    float diff = max(dot(vertNormal, lightDir), 0.0);

    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, vertNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    float dist = length(light.position - fragPosition);
    float attenuation = 1.0f / (pow(dist / light.radius, 2.0f) + 1.0f);

    vec3 ambient = (light.color * light.ambientStrength * material.ambient) * attenuation;
    vec3 diffuse = (light.color * (diff * material.diffuse)) * attenuation;
    vec3 specular = (light.color * (spec * material.specular)) * attenuation;

    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, Material material, vec3 vertNormal, vec3 fragPosition, vec3 viewPosition) {
    vec3 lightDir = normalize(light.position - fragPosition);

    float theta = dot(lightDir, normalize(-light.direction));
            
    if (theta > cos(light.angle)) {
        float diff = max(dot(vertNormal, lightDir), 0.0);

        vec3 viewDir = normalize(viewPosition - fragPosition);
        vec3 reflectDir = reflect(-lightDir, vertNormal);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

        float dist = length(light.position - fragPosition);
        float attenuation = 1.0f / (pow(dist / light.range, 2.0f) + 1.0f);

        float epsilon = cos(light.angle - light.blurAngle) - cos(light.angle);
        float intensity = clamp((theta - cos(light.angle)) / epsilon, 0.0f, 1.0f);

        vec3 diffuse = (light.color * (diff * material.diffuse)) * attenuation * intensity;
        vec3 specular = (light.color * (spec * material.specular)) * attenuation * intensity;

        return (diffuse + specular);
    } else {
        return vec3(0.0f, 0.0f, 0.0f);
    }
}

void main() {
    vec3 result = vertColor;
    if (lightable) {
        vec3 sum = vec3(0.0f, 0.0f, 0.0f);

        for (int i = 0; i < numberOfDirectionalLights; ++i) {
            sum += calculateDirectionalLight(directionalLights[i], material, vertNormal, viewPosition);
        }

        for (int i = 0; i < numberOfPointLights; ++i) {
            sum += calculatePointLight(pointLights[i], material, vertNormal, fragPosition, viewPosition);
        }

        for (int i = 0; i < numberOfSpotLights; ++i) {
            sum += calculateSpotLight(spotLights[i], material, vertNormal, fragPosition, viewPosition);
        }

        result = sum * vertColor;
    }

    FragColor = vec4(result, 1.0f);
}