function [param] = tuningScriptControllerIntegration(variant,verbose)
arguments
	variant='NREL126_5MW'
    verbose = false;
end
componentName = 'ControllerIntegration';

path = getVariantDataRoot;
varToLoad = {'GearRatio'};

modelData = load(fullfile(path,variant,strcat(variant,'.mat')),varToLoad{:}); 


parameters = genControllerIntegrationTuning(modelData,verbose);

% Write out parameters
paramNames = fieldnames(parameters);

for i = 1:length(paramNames)
    param(i,:) = {paramNames{i},parameters.(paramNames{i})};
end
    

param = [param repmat({componentName},length(paramNames),1)];
end