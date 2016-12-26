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
            'DisplayName','Cutoff Frequency','Label','Fc','Mapping',{'lin',0 10000}),...
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
	    A = max(max(x))*obj.A;
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

		    % m is the number of times the moog ladder is processed, choosing 1 for now
		    % (choosing m = 1:2 made it really glitchy)
		    for m = 1:1
			z = (x(n,:)-4*r*yprev(6,:))./(2*Vt);
		    	y(1,:) = yprev(1,:) + Vtx2xg*(tanh(z)-Wprev(1,:));

			z = y(1,:)/(Vtx2);

		    	%W(1,:) = tanh(z);
		    	W(1,:) = (z);

		    	y(2,:) = yprev(2,:) + Vtx2xg*(W(1,:)-Wprev(2,:));

			z = y(2,:)/Vtx2;

		    	%W(2,:) = tanh(z);
		    	W(2,:) = (z);

		    	y(3,:) = yprev(3,:) + Vtx2xg*(W(2,:)-Wprev(3,:));

			z = y(3,:)/(Vtx2);

		    	%W(3,:) = tanh(z);
		    	W(3,:) = (z);

			z = yprev(4,:)/(Vtx2);
		    	%y(4,:) = yprev(4,:) + Vtx2xg*(W(3,:)-tanh(z));
		    	y(4,:) = yprev(4,:) + Vtx2xg*(W(3,:)-(z));

			yprev(6,:) = (y(4,:) + yprev(5,:))*0.5;

			yprev(5,:) = y(4,:);

		    	yprev(1:4,:) = y(1:4,:);

		    	Wprev = W;
		end

		% prevent clipping in both channels
		if (yprev(6,1) > A)
			yprev(6,1) = A;
		end
		if (yprev(6,2) > A)
			yprev(6,2) = A;
		end
		out = [out;yprev(6,:)];
	    end
	    out = A*out/max(max(out));
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
