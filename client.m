function client(port)
%   provides a menu for accessing PIC32 motor control functions
%
%   client(port)
%
%   Input Arguments:
%       port - the name of the com port.  This should be the same as what
%               you use in screen or putty in quotes ' '
%
%   Example:
%       client('/dev/ttyUSB0') (Linux/Mac)
%       client('COM3') (PC)
%
%   For convenience, you may want to change this so that the port is hardcoded.
   
% Opening COM connection
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

fprintf('Opening port %s....\n',port);

% settings for opening the serial port. baud rate 230400, hardware flow control
% wait up to 120 seconds for data before timing out
mySerial = serial(port, 'BaudRate', 230400, 'FlowControl', 'hardware','Timeout',120); 
% opens serial connection
fopen(mySerial);
% closes serial port when function exits
clean = onCleanup(@()fclose(mySerial));                                 

has_quit = false;
% menu loop
while ~has_quit
    fprintf('PIC32 MOTOR DRIVER INTERFACE\n\n');
    % display the menu options; this list will grow
    fprintf('     a: Read current sensor (ADC counts)    b: Read current sensor (mA)\n');
    fprintf('     c: Encoder Angle (Count)               d: Encoder Angle (Degree)\n');    
    fprintf('     e: Encoder Reset                       q: Quit\n');
    % read the user's choice
    selection = input('\nENTER COMMAND: ', 's');
     
    % send the command to the PIC32
    fprintf(mySerial,'%c\n',selection);
    
    % take the appropriate action
    switch selection
        case 'a'                            % read adc count from current sensor
            counts = fscanf(mySerial,'%d'); 
            fprintf('The motor current is %d ADC counts.\n\n',counts);
            
        case 'b'                            % read current from current sensor in mA
            current = fscanf(mySerial,'%f'); % converted from adc count to mA
            fprintf('The motor current is %.2f mA.\n\n',current); 
            
        case 'c'                            % read encoder count
            counts = fscanf(mySerial,'%d'); % obtain encoder count
            fprintf('The motor angle is %d counts.\n\n',counts); 
            
        case 'd'                            % read encoder count in degrees
            degrees = fscanf(mySerial,'%f');% obtain encoder degree
            fprintf('The motor angle is %.2f degrees.\n\n',degrees);
            
        case 'e'                            % reset encoder count
            fprintf('Encoder count has been reset!\n\n');
        
        case 'f'                            % set PWM duty cycle and direction
            duty_cycle = input('What PWM value would you like [-100 to 100]? '); 
            fprintf(mySerial,'%d\n',duty_cycle);
            if (duty_cycle < 0)
                fprintf('PWM has been set to %d%% in the clockwise direction.\n',-1*duty_cycle);
            else
                fprintf('PWM has been set to %d%% in the counterclockwise direction.\n\n',duty_cycle);
            end
        case 'g'                            % Set current gains Kp and Ki
            kp = input('Enter your desired Kp current gain [recommended: 0.32]: ');
            fprintf(mySerial,'%f\n',kp);
            ki = input('Enter your desired Ki current gain [recommended: 0.041: ');
            fprintf(mySerial,'%f\n',ki);
            fprintf('\n');
            
        case 'h'
            gains = zeros(1,2);
            gains(1,:) = fscanf(mySerial, '%f %f');
            fprintf('The current controller is using Kp = %.4f and Ki = %.4f\n\n',gains(1),gains(2));
        
        case 'k'                            % Test current control and make plot
            fprintf('k mode\n');
            read_plot_matrix(mySerial);
            
        case 'p'                            % Unpower the motor
            fprintf('PIC32 controller switched to IDLE mode\n\n');
            
        case 'q'
            has_quit = true;                % exit client
            
        case 'r'                            % print out current mode 
            fprintf('scanning\n');
            mode_names = ["IDLE", "PWM", "ITEST", "HOLD", "TRACK"];
            mode = fscanf(mySerial,'%d');
            fprintf('The PIC32 controller mode is currently %s\n\n',mode_names(mode+1));
            
        otherwise
            fprintf('Invalid Selection %c\n\n', selection);
    end
end

end
