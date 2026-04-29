function [path] = getControllerIntegrationRoot()
path = mfilename('fullpath');
[path,~,~] = fileparts(path);
end

