function [pDef] = generateControllerIntegrationParam(pDef)
%% Parameter definition for ControllerIntegration.
% Input sequence: pDef,name,defaultValue,unit,dataType,tuningType,scaling,obfuscated
pDef = createParam(pDef,'GearRatio',1,'-','derived',1,0);
%pDef = createParam(pDef,'ThrustLim',1,'N','double','tunable',1,0); 

end

