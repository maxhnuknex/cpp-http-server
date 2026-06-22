
#pragma once


#include "IProjectPepository.h"
#include "../../Database/Database.h"

class SQLiteProjectRepository: public IProjectRepository{
    Database& database;
    Project toProject(sqlite3_stmt*);
public:
    SQLiteProjectRepository(Database& database);

    std::optional<Project> createProject(ProjectCreateCommand&) override;
    std::optional<Project> findById(int id) override;
    std::vector<Project> findAll() override;
    bool deleteProject(int id) override; 
};




    
