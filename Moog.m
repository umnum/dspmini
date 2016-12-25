classdef Moog < audioPlugin
    % Moog
    %           The effect delays the input signal from 0 - 1 second.
    %           Works in real-time and can generate an audio-plugin
    %           by using built-in functions from audio system toolbox
    % Input
    %           Delay: returns the delaytime in seconds
    %           Gain: Amplitude of the delayed signal, 0-1
    %           Feedback: how much of the delayed signal is feeded back into the
    %           effect. This should not be over 0.9 due to stability.
    %           Wet/Dry: How much of the original signal (dry ) and
    %           delayed signal (wet) is present in the out. Can mixed between 0-1.
    %           At 0 only the dry signal is present, at 1 the output is completely wet.
    % Effects
    
    %           Vibrato:
    %           Adds vibrato to the delayed signal. Can be controlled through
    %           'Vibrate Rate' and 'Vibrato Depth'
    %
    %           Reverse:
    %           Reverses the delayed signal.
    %
    %           Saturation:
    %           Distorts the delayed signal. The amount of distortion can be
    %           controlled with 'Saturation Amount'
    %
    %           HighPass and LowPass Filter:
    %           You can add a highpass or a lowpass filter to the delayed
    %           signal. The cutoff frequency can be controlled with the parameter Fc, and the
    %           quality of the filter can be controlled with Q.
    %
    %           Effects to implemented: Reverb?, grainular
    %           delay.
    properties
	fc = 5000;
	r = 0.5;
	g
	Vt = 2000000; % i chose a rediculously large number here to match
	              % values that are similar to the algorithm i found online
	yprev = [0 0;0 0;0 0;0 0;0 0;0 0];
	Wprev = [0 0;0 0;0 0];
    end
    
    properties (Dependent)
        %FeedbackLevel Feedback gain
        %   Specify the feedback gain value as a positive scalar. This
        %   value must range from 0 to 0.5. Setting FeedbackLevel to 0
        %   turns off the feedback. The default value of this property is
        %   0.35.
        FeedbackLevel = 0.35
        
    end
    
    properties (Constant)
        tTable = tanh(linspace(-1e-10,1e-10,2000000));
        % audioPluginInterface manages the number of input/output channels
        % and uses audioPluginParameter to generate plugin UI parameters.
        PluginInterface = audioPluginInterface(...
            'InputChannels',2,...
            'OutputChannels',2,...
            'PluginName','Moog Filter',...
            'VendorName', '', ...
            'VendorVersion', '3.1.4', ...
            'UniqueId', '4pvz',...
            audioPluginParameter('fc',...
            'DisplayName','Cutoff Frequency','Label','Fc','Mapping',{'lin',0 10000}),...
            audioPluginParameter('r',...
            'DisplayName','Resonance','Label','Res','Mapping',{'lin',0 1}));
    end
    
    properties (Access = private)
        %pSR Sample rate
	fs
    end
    
    methods
        % Constructor, called when initializing effect
        function obj = Moog()
            obj.fs = getSampleRate(obj);
	    obj.g = 1-exp(-2*pi*obj.fc/obj.fs);
        end
        
        % resets internal states of buffers
        function reset(obj)
            % reset sample rate
	    fs = getSampleRate(obj);
            obj.fs = fs;
	    % reset cutoff frequency
	    fc = 1000;
	    obj.fc = fc;
            % reset yprev
            obj.yprev = [0 0;0 0;0 0;0 0;0 0;0 0];
	    % reset Wprev
            obj.Wprev = [0 0;0 0;0 0];
	    % reset g
	    obj.g = 1-exp(-2*pi*fc/fs);
        end
        
        function set.fc(obj, fc)
            obj.fc = fc;
	    obj.g = 1-exp(-2*pi*fc/obj.fs);
        end
        
        function set.r(obj,r)
            obj.r = r;
        end

	function y = tanhTable(x)
	    y = tTable(round(100*(x+1)+1));
	end

	function [obj, out] = moogfilter(obj, x)
	    A = max(x);
	    out = [];
	    yprev = obj.yprev;
	    y = zeros(size(yprev));
	    Wprev = obj.Wprev;
	    W = zeros(size(Wprev));
	    r = obj.r;
	    Vt = obj.Vt;
	    Vtx2 = Vt*2;
	    Vtx2xg = Vtx2*obj.g;
	    for n = 1:length(x)

		    % number of times the moog ladder is processed, choosing 1 for now
		    % (choosing 2 made it really glitchy)
		    for m = 1:1
			z = (x(n,:)-4*r*yprev(6,:))./(2*Vt);
			%z = round(z*1e13)+1e6;
			%z = round(z*1.0e10+561185117);
			%disp(round(z*1.0e10+561185117));
		    	y(1,:) = yprev(1,:) + Vtx2xg*(tanh(z)-Wprev(1,:));

			z = y(1,:)/(Vtx2);
			%z = round(z*1e13)+1e6;
			%z = round(z*1.0e10+561185117)
			%disp(round(z*1.0e10+561185117));
		    	W(1,:) = tanh(z);

		    	y(2,:) = yprev(2,:) + Vtx2xg*(W(1,:)-Wprev(2,:));

			z = y(2,:)/Vtx2;
			%z = round(z*1e13)+1e6;
			%z = round(z*1.0e10+561185117);
			%disp(round(z*1.0e10+561185117));
		    	W(2,:) = tanh(z);

		    	y(3,:) = yprev(3,:) + Vtx2xg*(W(2,:)-Wprev(3,:));

			z = y(3,:)/(Vtx2);
			%z = round(z*1e13)+1e6;
			%z = round(z*1.0e10+561185117);
			%disp(round(z*1.0e10+561185117));
		    	W(3,:) = tanh(z);

			z = yprev(4,:)/(Vtx2);
			%z = round(z*1e13)+1e6;
			%z = round(z*1.0e10+561185117);
			%disp(round(z*1.0e10+561185117));
		    	y(4,:) = yprev(4,:) + Vtx2xg*(W(3,:)-tanh(z));

			yprev(6,:) = (y(4,:) + yprev(5,:))*0.5;

			yprev(5,:) = y(4,:);

		    	yprev(1:4,:) = y(1:4,:);

		    	Wprev = W;
		end
		out = [out;yprev(6,:)];
	    end
	    maxout = max(out);
	    out = A(1)*out/maxout(1);
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
