/* COP 3502C Assignment 1
    This program is written by Zainab Syed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 22

typedef struct RegisteredVehicle{ //for one registered vehicle
    char* license_plate; //to be used for string license plate# of the vehicle
    char* owner_name; //to be used for storing owner name
} RegisteredVehicle;

typedef struct Garage{ //for one garage
    char* garage_name; //to be used for garage name
    int total_capacity; //total capacity of the garage
    int current_count; // number of vehicles in the garage in a particular time
    RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct Campus{
    Garage** garages; //list of garages in campus
    int total_garages; //number of garages in the campus
    RegisteredVehicle** registered_vehicles; //list of registered vehicles
    int total_registered_vehicles; //total number of registered vehicles
} Campus;

/* Creates a new garage with the specified name and capacity and initializes
    other variables and makes parked vehicles array based on the capacity. */
Garage* createGarage(const char* name, int capacity)
{
    Garage* gInfo = (Garage*)malloc(sizeof(Garage));

    // allocate space for the garage name and copy the name to garage_name
    gInfo->garage_name = (char*)malloc(sizeof(strlen(name) + 1));
    strcpy(gInfo->garage_name, name);

    gInfo->total_capacity = capacity; // capacity of garage
    gInfo->current_count = 0; // initializes garage to 0 cars

    // creates an array with the size of RegisteredVehicles to store info on every vehicle in the garage
    gInfo->parked_vehicles = (RegisteredVehicle**)malloc(capacity * sizeof(RegisteredVehicle*));

    return gInfo;
}

/* Create a new vehicle with the given license plate and owner.*/
RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner)
{
    RegisteredVehicle* rV = (RegisteredVehicle*)malloc(sizeof(RegisteredVehicle));

    rV->license_plate = (char*)malloc(sizeof(strlen(license)+1));
    strcpy(rV->license_plate, license);

    rV->owner_name = (char*)malloc(sizeof(strlen(owner)+1));
    strcpy(rV->owner_name, owner);

    return rV;
}

// registers a new vehicle on campus
void registerVehicle(Campus* campus, const char* license, const char* owner)
{
    // if license plate does not exist, add it to the list of registered vehicles
    RegisteredVehicle* newVehicle = createRegisteredVehicle(license, owner);

    campus->registered_vehicles = (RegisteredVehicle**)realloc(campus->registered_vehicles, (campus->total_registered_vehicles + 1) * sizeof(RegisteredVehicle*));
    campus->registered_vehicles[campus->total_registered_vehicles] = newVehicle;
    campus->total_registered_vehicles++;
    printf("REGISTERED\n");
}

/* Parks a vehicle in a specified garage. If the garage is full, it prints “FULL”.
    Otherwise, if the parking is successful, it prints “PARKED”. */
void parkVehicle(Garage* garage, RegisteredVehicle* vehicle)
{
    // if the garage is full it prints FULL
    if(garage->current_count >= garage->total_capacity)
    {
        printf("FULL\n");
    }
    // adds the vehicle to the last index of the parked vehicles in the garage
    // increases the current count by 1 and prints PARKED
    else
    {
        garage->parked_vehicles = (RegisteredVehicle**)realloc(garage->parked_vehicles, (garage->current_count+1)*sizeof(RegisteredVehicle*));
        garage->parked_vehicles[garage->current_count] = vehicle;
        garage->current_count++;
        printf("PARKED\n");
    }
}

/* Removes a vehicle from a specified garage. The function returns 1 if the
    removal is successful. Otherwise, it returns 0. */
int removeVehicleFromGarage(Garage* garage, const char* license)
{
    int carfound = 0;
    for (int i = 0; i < garage->current_count; i++)
    {
        if (strcmp(garage->parked_vehicles[i]->license_plate, license) == 0)
        {
            carfound = 1;
            free(garage->parked_vehicles[i]->license_plate);
            free(garage->parked_vehicles[i]->owner_name);
            free(garage->parked_vehicles[i]);
            for(int j = i; j < garage->current_count - 1; j++) {
                garage->parked_vehicles[j] = garage->parked_vehicles[j+1];
            }
            garage->current_count--;

            return 1;
        }
    }
    // the car isn't in this garage and returns 0
    if(carfound == 0){

        return 0;
    }
}


/* Searches for a vehicle by its license plate across all garages on campus. If
    the vehicle is found, it return the vehicle. Otherwise, it should return NULL. */
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license)
{
    // go through all of the garage
        // in each garage go through all the registered vehicle
            // for every vehicle check the license plate using strcmp
    for (int i = 0; i < campus->total_garages; i++)
    {
        for(int j = 0; j < campus->garages[i]->current_count; j++)
        {
            if(strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, license)==0)
            {
                // printf("1");
                return campus->garages[i]->parked_vehicles[j];
            }
        }
    }
    return NULL;

}

/* Returns the total number of vehicles parked across all garages on campus.*/
int countTotalVehicles(const Campus* campus)
{
    int total_cars = 0;
    for (int i = 0; i < campus->total_garages; i++) {
        total_cars+= campus->garages[i]->current_count;
    }
    return total_cars;
}

/* Resizes a garage to a new capacity. If the new capacity is smaller than the
    current number of vehicles in that garage, then you should print a massage
    "FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n". The function returns 1
    or 0 depending on whether the task was successful or not. */
int resizeGarage(Garage* garage, int new_capacity)
{
    if(new_capacity < garage->current_count)
    {
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;
    }
    else
    {
        garage->parked_vehicles = (RegisteredVehicle**)realloc(garage->parked_vehicles,
                                            new_capacity*sizeof(RegisteredVehicle*));
        garage->total_capacity = new_capacity;
        return 1;
    }
}

/* Relocates a vehicle from one garage to another. The function returns 1 or 0
    depending on whether the task was successful or not. */
int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name)
{
    Garage * t_garage;
    Garage * source_garage;
    int garage_found = 0;
    RegisteredVehicle* target_vehicle;

    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        if(strcmp(campus->registered_vehicles[i]->license_plate, license) == 0){
            target_vehicle = searchVehicleByLicense(campus, license);
            break;
        }
    }

    for(int i = 0; i < campus->total_garages; i++)
    {
        for(int j = 0; j < campus->garages[i]->current_count; j++)
        {
            if(strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, license) == 0)
            {
                source_garage = campus->garages[i];
                break;
            }
        }
    }

    for (int i = 0; i < campus->total_garages; i++) {
        if(strcmp(campus->garages[i]->garage_name, target_garage_name) == 0) {
            t_garage = campus->garages[i];
            garage_found = 1;
            break;
        }
    }

    if (garage_found != 1) {
        printf("%s NOT FOUND.\n", target_garage_name);
        return 0;
    }

    if(target_vehicle == NULL) {
        printf("%s NOT IN CAMPUS.\n", license);
        return 0;
    }

    if(t_garage->current_count >= t_garage->total_capacity)
    {
        printf("%s IS FULL.\n", t_garage->garage_name);
         return 0;
    }

    else {
        int removed = removeVehicleFromGarage(source_garage, license);
        parkVehicle(t_garage, target_vehicle);
        printf("RELOCATION SUCCESSFUL.\n");
        return 1;
    }
}


/* Displays all vehicles owned by a specified person if they are in campus. */
void displayVehiclesByOwner(const Campus* campus, const char* owner_name)
{
    int vehicle_found = 0;  // Flag to check if any vehicles belong to the owner

    // Loop through all registered vehicles
    for (int i = 0; i < campus->total_registered_vehicles; i++) {
        RegisteredVehicle* vehicle = campus->registered_vehicles[i];

        // Check if the vehicle belongs to the specified owner
        if (strcmp(vehicle->owner_name, owner_name) == 0) {
            vehicle_found = 1;  // Mark that at least one vehicle was found

            // Check if the vehicle is parked in any garage
            int is_parked = 0;
            for (int j = 0; j < campus->total_garages; j++) {
                Garage* garage = campus->garages[j];

                // Loop through all vehicles in the current garage
                for (int k = 0; k < garage->current_count; k++) {
                    if (strcmp(garage->parked_vehicles[k]->license_plate, vehicle->license_plate) == 0) {
                        // The vehicle is parked in this garage
                        printf("%s %s\n", vehicle->license_plate, garage->garage_name);
                        is_parked = 1;
                        break;
                    }
                }
                if (is_parked) break;  // If the vehicle is parked, no need to check other garages
            }

            // If the vehicle was not found in any garage
            if (is_parked == 0) {
                printf("%s NOT ON CAMPUS\n", vehicle->license_plate);
            }
        }
    }

    // If no vehicles were found for the owner
    if (!vehicle_found) {
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }
}
/* Removes a specified garage from the campus and frees the memory. Note that
    while removing a garage, you should not remove the vehicles from
    the system. The function returns 1 or 0 depending on whether the task was
    successful or not.*/
int removeGarage(Campus* campus, const char* garage_name)
{
    for (int i = 0; i < campus->total_garages; i++)
    {
        if(strcmp(campus->garages[i]->garage_name, garage_name) == 0)
        {
            // for(int j = 0; j < campus->garages[i]->total_capacity; j++)
            //{
            //    free(campus->garages[i]->parked_vehicles[j]);

            //}
            // free(campus->garages[i]->parked_vehicles);
            free(campus->garages[i]->garage_name);
            free(campus->garages[i]);

            for(int k = i; k < campus->total_garages-1; k++)
            {
                campus->garages[k] = campus->garages[k+1];
            }

            campus->total_garages--;
            printf("%s REMOVED\n", garage_name);
            return 1;
        }
    }
    printf("%s NOT FOUND\n", garage_name);
    return 0;
}
/* Generates a report on the utilization of all garages. For each garage, it
    should print: “Garage: [G], Capacity: [GC], Occupied: [GO],
    Utilization: [GU]%”, where [G] is the garage name, [GC] is the capacity of
    the garage, [GO] is the count of vehicles in the garage, and [GU] is the
    percentage of fullness of the garage. After printing the above summary,
    you should print the name of garage which is utilized least at that time. If multiple garages have the same
minimum percentage of the utilization, then you should print the one that came first in the inputs*/
void generateGarageUtilizationReport(const Campus* campus)
{
    double minUtilization = (double)campus->garages[0]->current_count / campus->garages[0]->total_capacity;
    char garage_minUtilization[22];
    strcpy(garage_minUtilization, campus->garages[0]->garage_name);

    for(int i = 0; i < campus->total_garages; i++)
    {
        double utilization = ((double)(campus->garages[i]->current_count))/(campus->garages[i]->total_capacity);
        if(minUtilization > utilization) {
            minUtilization = utilization;
            strcpy(garage_minUtilization, campus->garages[i]->garage_name);
        }
    }
    for(int i = 0; i < campus->total_garages; i++) {
        double utilization = ((double)(campus->garages[i]->current_count))/(campus->garages[i]->total_capacity);
        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2f%%\n",
               campus->garages[i]->garage_name, campus->garages[i]->total_capacity,
                    campus->garages[i]->current_count, utilization*100);

    }
    printf("Least Utilized: %s\n", garage_minUtilization);
}

void free_mem(Campus* campus)
{
    for(int i = 0; i < campus->total_garages; i++)
    {
        /// CAUSED DOUBLE FREE
        //for(int j = 0; j < campus->garages[i]->current_count; j++)
        //{
        //    free(campus->garages[i]->parked_vehicles[j]->license_plate);
        //    free(campus->garages[i]->parked_vehicles[j]->owner_name);
        //}
        free(campus->garages[i]->parked_vehicles);
        free(campus->garages[i]->garage_name);
        free(campus->garages[i]);
    }
    free(campus->garages);
    /* accidentally double freed
    for(int i = 0; i < campus->total_registered_vehicles; i++)
    {
        free(campus->registered_vehicles[i]->license_plate);
        free(campus->registered_vehicles[i]->owner_name);
    }
    */
    free(campus->registered_vehicles);
}

int main()
{
    Campus* campus = (Campus*)malloc(sizeof(Campus));
    int num_commands;

    // gets input for the # of garages, # of cars initially, and # of commands
    scanf("%d %d %d", &campus->total_garages, &campus->total_registered_vehicles, &num_commands);

    // creates an array of garages
    campus->garages = (Garage**)malloc(campus->total_garages * sizeof(Garage*));

    // creates an array of the total amount of vehicles
    campus->registered_vehicles = (RegisteredVehicle**)malloc(campus->total_registered_vehicles * sizeof(RegisteredVehicle*));

    // alocates memory, gets name and capacity for each garage. calls createGarage
    for (int i = 0; i < campus->total_garages; i++)
    {
        campus->garages[i] = (Garage*)malloc(sizeof(Garage));
        char garageName[STR_MAX];
        int garageCapacity;
        scanf("%s %d", garageName, &garageCapacity);
        campus->garages[i] = createGarage(garageName, garageCapacity);
    }

    // allocates memory for each car. Takes input for the license and owner name. calls createRegisteredVehicle
    for (int j = 0; j < campus->total_registered_vehicles; j++)
    {
        campus->registered_vehicles[j] = (RegisteredVehicle*)malloc(sizeof(RegisteredVehicle));
        char licensePlate[STR_MAX];
        char owner[STR_MAX];
        scanf("%s %s", licensePlate, owner);
        campus->registered_vehicles[j] = createRegisteredVehicle(licensePlate, owner);
    }

    // loops and takes inputs for the # of commands
    for (int x = 0; x < num_commands; x++)
    {
        // takes in a command to prompt a function
        char command[STR_MAX];
        scanf("%s", command);

        // calls function parkVehicle
        if (strcmp(command, "PARK") == 0)
        {
            Garage* garage;
            RegisteredVehicle* vehicle;
            char license[STR_MAX];
            char g_name[STR_MAX];
            // printf("License and Garage Name\n");
            scanf("%s %s\n", license, g_name);

            // finds the garage in the array of garages in struct campus
            for(int i = 0; i < campus->total_garages; i++)
            {
                if(strcmp(campus->garages[i]->garage_name, g_name) == 0)
                {
                    garage = campus->garages[i];
                    break;
                }
            }
            // finds the vehicle in the array of registered_vehicles in struct campus
            for(int j = 0; j < campus->total_registered_vehicles; j++)
            {
                if(strcmp(campus->registered_vehicles[j]->license_plate, license)==0)
                    {
                        vehicle = campus->registered_vehicles[j];
                        break;
                    }
            }
            // calls function parkVehicle passing the garage and vehicle
            parkVehicle(garage, vehicle);
        }

        // calls function generateGarageUtilizationReport
        else if (strcmp(command, "UTILIZATION_REPORT") == 0)
        {
            generateGarageUtilizationReport(campus);
        }

        // calls function resizeGarage
        else if(strcmp(command, "RESIZE")==0)
        {
            Garage* r_garage;
            char target_garage[STR_MAX];
            int new_capacity;

            scanf("%s %d \n", target_garage, &new_capacity);

            for(int i = 0; i < campus->total_garages; i++)
            {
                if(strcmp(campus->garages[i]->garage_name, target_garage) == 0)
                {
                    r_garage = campus->garages[i];
                }
            }
            int resizeResult = resizeGarage(r_garage, new_capacity);
            if (resizeResult == 1)
            {
                printf("SUCCESS\n");
            }
        }

        // calls function displayVehicleByOwner
        else if(strcmp(command, "SEARCH_OWNER")==0)
        {
            char owner[STR_MAX];
            scanf("%s", owner);
            displayVehiclesByOwner(campus, owner);
        }

        // calls function  relocateVehicle
        else if (strcmp(command, "RELOCATE")==0)
        {
            char move_license[STR_MAX];
            char target_garage[STR_MAX];
            scanf("%s %s", move_license, target_garage);

            int result = relocateVehicle(campus, move_license, target_garage);
            /*if (result == 1)
            {
                printf("PARKED \nRELOCATION SUCCESSFUL");
            } */
        }

        // calls countTotalVehicles
        else if(strcmp(command, "COUNT_TOTAL")==0)
        {
            int total_cars = countTotalVehicles(campus);
            printf("%d\n", total_cars);
        }

        // calls registerVehicle. Registers a new vehicle
        else if(strcmp(command, "REGISTER_VEHICLE")==0)
        {
            char new_license[STR_MAX];
            char new_owner[STR_MAX];
            scanf("%s %s \n", new_license, new_owner);
            registerVehicle(campus, new_license, new_owner);
        }

        // calls removeVehicleFromGarage. Removes a vehicle from garage
        else if(strcmp(command, "REMOVE_VEHICLE_GARAGE")==0)
        {
            char r_license[STR_MAX];
            Garage* source_garage = (Garage*)malloc(sizeof(Garage));
            scanf("%s", r_license);

            // finds garage the car is in
            for (int i = 0; i < campus->total_garages; i++)
            {
                for (int j = 0; j < campus->garages[i]->current_count; j++)
                {
                    if(strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, r_license)==0)
                    {
                        source_garage = campus->garages[i];
                        break;
                    }
                }
            }
            int removal_result = removeVehicleFromGarage(source_garage, r_license);
            if (removal_result == 1)
            {
                printf("REMOVED FROM %s\n", source_garage->garage_name);
            }
            else
            {
                printf("NOT FOUND IN CAMPUS\n");
            }
        }

        // calls removeGarage
        else if(strcmp(command, "REMOVE_GARAGE")==0)
        {
            char garage_removed[STR_MAX];
            scanf("%s", garage_removed);
            int r_G_result = removeGarage(campus, garage_removed);
        }
    }
    free_mem(campus);
}
