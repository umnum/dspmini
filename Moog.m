classdef Moog < audioPlugin
    % Moog
    %           The Moog class is a real-time implementation of the 
    %           Moog ladder filter. This implementation is based on 
    %           Huovilainen's 2004 paper 'Non-Linear Digital Implementation
    %           of the Moog Ladder filter'
    % Input
    %           A: The amplitude gain of the filter's output
    %           Fc: The filter's cutoff frequency 
    %           R: The filter's resonance
    properties
	fc = 5000;
	r = 0.5;
	A = 0.5;
	g
	Vt = 2000000; % Thermal Voltage, value should technically be 25 mV
	              % I chose a rediculously large number here to match
	              % values that are similar to the algorithm i found online
	% unit delays
	yprev = [0 0;0 0;0 0;0 0;0 0;0 0];
	Wprev = [0 0;0 0;0 0];
    end
    
    properties (Dependent)
    end
    
    properties (Constant)

        % audioPluginInterface manages the number of input/output channels
        % and uses audioPluginParameter to generate plugin UI parameters.
        PluginInterface = audioPluginInterface(...
            'InputChannels',2,...
            'OutputChannels',2,...
            'PluginName','Moog Filter',...
            'VendorName', '', ...
            'VendorVersion', '3.1.4', ...
            'UniqueId', '4pvz',...
            audioPluginParameter('A',...
            'DisplayName','Amplitude','Label','Amp','Mapping',{'lin',0 1}),...
            audioPluginParameter('fc',...
            'DisplayName','Cutoff Frequency','Label','Fc','Mapping',{'lin',100 10000}),...
            audioPluginParameter('r',...
            'DisplayName','Resonance','Label','Res','Mapping',{'lin',0 1}));
    end
    
    properties (Access = private)
        % sample rate
	fs
    end
    
    methods
        % Constructor, called when initializing effect
        function obj = Moog()
            obj.fs = getSampleRate(obj);
	    obj.g = 1-exp(-2*pi*obj.fc/obj.fs);
        end
        
        % reset parameter values and internal buffers
        function reset(obj)
            % reset sample rate
	    fs = getSampleRate(obj);
            obj.fs = fs;
	    % reset cutoff frequency
	    fc = 1000;
	    %obj.fc = fc; % comment this line out when exporting to VST
            % reset yprev
            obj.yprev = [0 0;0 0;0 0;0 0;0 0;0 0];
	    % reset Wprev
            obj.Wprev = [0 0;0 0;0 0];
	    % reset g
	    obj.g = 1-exp(-2*pi*fc/fs);
        end
        
	% set the output amplitude gain
        function set.A(obj, A)
            obj.A = A;
        end

	% set the cutoff frequency and g
        function set.fc(obj, fc)
            obj.fc = fc;
	    obj.g = 1-exp(-2*pi*fc/obj.fs);
        end
        
	% set the resonance
        function set.r(obj,r)
            obj.r = r;
        end

	% Moog filter implementation
	function [obj, out] = moogfilter(obj, x)

	    % storing class variables as local variables
	    A = obj.A;
	    yprev = obj.yprev;
	    y = zeros(size(yprev));
	    Wprev = obj.Wprev;
	    W = zeros(size(Wprev));
	    r = obj.r;
	    Vt = obj.Vt;
	    Vtx2 = Vt*2;
	    Vtx2xg = Vtx2*obj.g;

	    % initialize output buffer
	    out = [];

	    % process the input buffer
	    for n = 1:length(x)

	    	% m is the number of times the moog ladder is processed
		for m = 1:2

		    % the first stage of the Moog ladder filter
   	   	    theta = (x(n,:)-4*r*yprev(6,:))./(2*Vt);
		    y(1,:) = yprev(1,:) + Vtx2xg*(tanh(theta)-Wprev(1,:));

		    % the next two stages of the filter
		    for k = 1:3
			theta = y(k,:)/(Vtx2);
			W(k,:) = tanh(theta);
			    if (k~=3) % the final stage is calculated differently
				y(k+1,:) = yprev(k+1,:) + Vtx2xg*(W(k,:)-Wprev(k+1,:));
			    end
  		    end

		    % the final stage of the filter
		    theta = yprev(4,:)/(Vtx2);
		    y(4,:) = yprev(4,:) + Vtx2xg*(W(3,:)-tanh(theta));

		    % adjust the output phase
		    yprev(6,:) = (y(4,:) + yprev(5,:))*0.5;

		    yprev(5,:) = y(4,:);

		    % update the unit delays
		    yprev(1:4,:) = y(1:4,:);
		    Wprev = W;
		end

		% prevent clipping in both channels
		[yprevlength,numchannels] = size(yprev);
		for n = 1:numchannels
			if (yprev(6,n) > A)
				yprev(6,n) = A;
			end
		end
		out = [out;yprev(6,:)];
	    end
	    out = A.*out./(max(out));
	    obj.yprev = yprev;
	    obj.Wprev = Wprev;
	end
        
        % output function, gets called at buffer speed
        function y = process(obj, x)
            % calculate moog filter
            [~, y] = moogfilter(obj, x);
        end
    end
end
