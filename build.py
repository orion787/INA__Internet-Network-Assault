import os
import subprocess

def build_project():
    # Определяем пути
    project_dir = os.getcwd()
    src_dir = os.path.join(project_dir, "src")
    build_dir = os.path.join(project_dir, "build")
    output_file = os.path.join(build_dir, "ddos_simulator")

    # Создаём директорию build, если она не существует
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
        print(f"Created build directory: {build_dir}")

    # Команда сборки
    build_command = (
        f"g++ -std=c++14 main.cpp "
        f"attacks/HttpFlood.cpp attacks/TcpSynFlood.cpp attacks/UdpFlood.cpp "
        f"core/ThreadPool.cpp core/AsyncClient.cpp core/Config.cpp "
        f"utils/Logger.cpp utils/ReportGenerator.cpp "
        f"-I./../include/attacks -I./../include/core -I../include/utils "
        f"-o {output_file} -pthread"
    )

    # Выполнение команды сборки
    try:
        print(f"Building project in directory: {src_dir}")
        subprocess.run(build_command, cwd=src_dir, shell=True, check=True)
        print(f"Build completed successfully! Executable created at: {output_file}")
    except subprocess.CalledProcessError as e:
        print(f"Build failed with error: {e}")

if __name__ == "__main__":
    build_project()
