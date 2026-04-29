load('BasisSimulinkConfig.mat')

% Load bus definition
load('InitValsBus.mat')
load('SensorBus.mat');
load('SafeSensorBus.mat');
load('ControllerOutputBus.mat');
load('Dq0Bus.mat');

%%
Const.Ts                        = 0.02;
Const.EnableSimulinkInit        = 1;

%% Simulator Parameters

modelData = DefineModelData();

simParam = [];
simParam = mergeStructs(simParam,genPowerTrainTuning(modelData));
simParam = mergeStructs(simParam,genPitchSystemTuning(modelData));

Sim = genTurbineModelParameters(simParam,modelData);

Rad2Deg = modelData.Rad2Deg;


%% Controller parameters
[Param,ParamStruct]=createParamStructFromDB();

