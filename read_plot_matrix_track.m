function data = read_plot_matrix_track(mySerial)
  nsamples = fscanf(mySerial,'%d');       % first get the number of samples being sent
  data = zeros(nsamples,2);               % two values per sample:  ref and actual
  for i=1:nsamples
    data(i,:) = fscanf(mySerial,'%f %f'); % read in data from PIC32; in degrees
    times(i) = (i-1)*0.005;                 % 0.005 s between samples
  end
  if nsamples > 1						        
    stairs(times,data(:,1:2));            % plot the reference and actual
  else
    fprintf('Only 1 sample received\n')
    disp(data);
  end
  % compute the average error
  score = mean(abs(data(:,1)-data(:,2)));
  fprintf('\nAverage error: %5.1f degrees\n',score);
  title(sprintf('Average error: %5.1f degrees',score));
  ylabel('Angle (degrees)');
  xlabel('Time (s)');  
end
