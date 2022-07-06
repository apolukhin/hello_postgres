import pathlib

import pytest

from testsuite.databases.pgsql import discover


pytest_plugins = [
    'pytest_userver.plugins',
    'testsuite.databases.pgsql.pytest_plugin',
]


@pytest.fixture(scope='session')
def root_dir():
    """Path to root directory service."""
    return pathlib.Path(__file__).parent.parent


@pytest.fixture(scope='session')
def pgsql_local(root_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'uservice_dynconf',
        [root_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))


@pytest.fixture
def client_deps(pgsql):
    pass
